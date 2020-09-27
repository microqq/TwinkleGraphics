
#include "twMesh.h"
#include "twConsoleLog.h"

namespace TwinkleGraphics
{


void SubMesh::Initialize(int32 indice_num)
{
    if(indice_num != 0 && _indice == nullptr)
    {
        _indiceNum = indice_num;
        _indice = new glm::uint32[indice_num];
    }
}

Mesh::~Mesh()
{
    Console::LogInfo("Mesh: Mesh Deconstruct.\n");

    _submeshes.clear();

    SAFE_DEL(_verticePos);
    SAFE_DEL(_verticeColor);
    SAFE_DEL(_verticeBinormal);
    SAFE_DEL(_verticeNormal);
    SAFE_DEL(_verticeUV);
    SAFE_DEL(_verticeUV1);
    SAFE_DEL(_verticeUV2);
    SAFE_DEL(_verticeUV3);
    SAFE_DEL(_verticeUV4);
    SAFE_DEL(_verticeUV5);
    SAFE_DEL(_verticeUV6);
    SAFE_DEL(_verticeUV7);
}

/**
 * @brief 创建球面网格的基础算法（https://zh.wikipedia.org/wiki/%E7%90%83%E5%BA%A7%E6%A8%99%E7%B3%BB）
 * 球坐标系由点 p 到原点 o 的距离 r，极角（pole angle）和方位角（azimuth angle）构成，若已知这三个变量，
 * 可通过球坐标系转直角坐标系求得其直角坐标（x,y,z）
 * @param radius
 * @param longitude_count
 * @param latitude_count
 * @return Mesh::Ptr
 */
Mesh::Ptr Mesh::CreateSphereMeshStandard(float32 radius, int32 longitude_count, int32 latitude_count)
{
    int32 row_count = latitude_count - 1;
    int32 col_count = longitude_count;
    int32 num = col_count * row_count + 2;

    Mesh::Ptr mesh = std::make_shared<Mesh>();
    mesh->Initialize(num, MeshDataFlag::DEFAULT);

    int indice_num = 6 * col_count + (row_count - 1) * col_count * 6;
    SubMesh::Ptr submesh = std::make_shared<SubMesh>();
    submesh->Initialize(indice_num);
    mesh->AddSubMesh(submesh);

    // submesh->Initialize(num, 0, 0, MeshDataFlag::DEFAULT);

    float32 u_step = glm::two_pi<float32>() / (float32)(longitude_count - 1);
    float32 v_step = glm::pi<float32>() / (float32)latitude_count;

    //pole points
    mesh->_verticePos[0].x = 0.0f;
    mesh->_verticePos[0].y = radius;
    mesh->_verticePos[0].z = 0.0f;

    mesh->_verticePos[num - 1].x = 0.0f;
    mesh->_verticePos[num - 1].y = -radius;
    mesh->_verticePos[num - 1].z = 0.0f;

    //compute vertice position
    float32 x, y, z;
    for(int32 i = 0, k = 1; i < row_count; i++)
    {
        y = radius * cos(v_step * (i + 1));
        for(int32 j = 0; j < col_count; j++)
        {
            x = radius * sin(v_step * (i + 1)) * cos(u_step * j);
            z = radius * sin(v_step * (i + 1)) * sin(u_step * j);

            mesh->_verticePos[k++] = glm::vec3(x, y, z);
        }
    }

    for(int32 i = 0, j = 0; i < num - 2; i++)
    {
        int32 row = i / col_count;
        int32 col = i % col_count;
        if(0 == row)
        {
            if(col == col_count - 1)
            {
                submesh->_indice[j] = row * col_count + col + 1;
                submesh->_indice[j + 1] = row * col_count + 1;
                submesh->_indice[j + 2] = 0;
            }
            else
            {
                submesh->_indice[j] = row * col_count + col + 1;
                submesh->_indice[j + 1] = row * col_count + col + 2;
                submesh->_indice[j + 2] = 0;
            }

            j += 3;
        }
        else
        {
            int32 top_row = row - 1;
            if(col == col_count - 1)
            {
                submesh->_indice[j] = row * col_count + col + 1;
                submesh->_indice[j + 1] = top_row * col_count + 1;
                submesh->_indice[j + 2] = top_row * col_count + 1;

                submesh->_indice[j + 3] = row * col_count + col + 1;
                submesh->_indice[j + 4] = row * col_count + 1;
                submesh->_indice[j + 5] = top_row * col_count + 1;
            }
            else
            {
                submesh->_indice[j] = row * col_count + col + 1;
                submesh->_indice[j + 1] = top_row * col_count + col + 2;
                submesh->_indice[j + 2] = top_row * col_count + col + 1;

                submesh->_indice[j + 3] = row * col_count + col + 1;
                submesh->_indice[j + 4] = row * col_count + col + 2;
                submesh->_indice[j + 5] = top_row * col_count + col + 2;
            }

            j += 6;

            if(row == row_count - 1)
            {
                if(col == col_count - 1)
                {
                    submesh->_indice[j] = row * col_count + col + 1;
                    submesh->_indice[j + 1] = num - 1;
                    submesh->_indice[j + 2] = row * col_count + 1;
                }
                else
                {
                    submesh->_indice[j] = row * col_count + col + 1;
                    submesh->_indice[j + 1] = num - 1;
                    submesh->_indice[j + 2] = row * col_count + col + 2;
                }

                j += 3;
            }
        }
    }

    return mesh;
}

/**
 * @brief 单位立方体均匀细分法（uniform subdive normalized cube）
 * 根据细分程度递归细分立方体六个面，新的顶点归一化，最后将得到顶点坐标乘以球半径 r
 *
 * @param radius
 * @param subdivide
 * @return Mesh::Ptr
 */
Mesh::Ptr Mesh::CreateSphereMeshNormalizedCube(float32 radius, int32 subdivide)
{
    int32 row_count, col_count;
    row_count = col_count = subdivide + 1;
    int32 num = row_count * col_count * 6;

    Mesh::Ptr mesh = std::make_shared<Mesh>();
    mesh->Initialize(num, MeshDataFlag::DEFAULT);

    int32 indiceNum = subdivide * subdivide * 36;
    SubMesh::Ptr submesh = std::make_shared<SubMesh>();
    submesh->Initialize(indiceNum);

    mesh->AddSubMesh(submesh);

    //cube faces
    glm::vec3 faces[24] =
    {
        /**
         * @brief
         * 0 __ __ __ 3
         *  |        |
         *  |        |
         *  |__ __ __|
         * 1          2
         */

        //front face
        glm::vec3(-0.5f, 0.5f, 0.5f),
        glm::vec3(-0.5f, -0.5f, 0.5f),
        glm::vec3(0.5f, -0.5f, 0.5f),
        glm::vec3(0.5f, 0.5f, 0.5f),

        //left face
        glm::vec3(-0.5f, 0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f, 0.5f),
        glm::vec3(-0.5f, 0.5f, 0.5f),

        //back face
        glm::vec3(0.5f, 0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3(-0.5f, 0.5f, -0.5f),

        //right face
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.5f, -0.5f, 0.5f),
        glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f, 0.5f, -0.5f),

        //top face
        glm::vec3(-0.5f, 0.5f, -0.5f),
        glm::vec3(-0.5f, 0.5f, 0.5f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.5f, 0.5f, -0.5f),

        //bottom face
        glm::vec3(-0.5f, -0.5f, 0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f, 0.5f)
    };

    //compute vertice & indice
    int32 vertice_index = 0;
    int32 element_index = 0;
    int32 face_index = -1;
    float32 step = 1.0f / subdivide;
    for(int32 i = 0; i < 24; i += 4)
    {
        face_index = i / 4;
        vertice_index = face_index * row_count * col_count;
        for(int32 j = 0; j < row_count; j++)
        {
            for(int32 k = 0; k < col_count; k++)
            {
                //double linear interpolation
                glm::vec3 col_temp_1 = (faces[i + 3] - faces[i]) * (float32)k * step + faces[i];
                glm::vec3 col_temp_2 = (faces[i + 2] - faces[i + 1]) * (float32)k * step + faces[i + 1];
                glm::vec3 coord = (col_temp_2 - col_temp_1) * (float32)j * step + col_temp_1;
                coord = glm::normalize(coord);
                coord *= radius;

                // submesh->_indice[element_index++] = vertice_index;

                if(j != row_count - 1 && k != col_count - 1)
                {
                    submesh->_indice[element_index] = vertice_index;
                    submesh->_indice[element_index + 1] = vertice_index + col_count;
                    submesh->_indice[element_index + 2] = vertice_index + col_count + 1;

                    submesh->_indice[element_index + 3] = vertice_index;
                    submesh->_indice[element_index + 4] = vertice_index + col_count + 1;
                    submesh->_indice[element_index + 5] = vertice_index + 1;

                    element_index += 6;
                }

                mesh->_verticePos[vertice_index++] = coord;
            }
        }
    }

    return mesh;
}


/**
 * @brief 细分Icosahedron
 * Icoshahedron是一个12个顶点的20面体，细分的方法是每一个三角形拆解成四个三角形
 *
 *         /\               /\
 *        /  \             /__\
 *       /    \   -----   /\  /\
 *      /__  __\         /__\/__\
 *
 * 球坐标系下， 首先生成一个正二十面体， 除两个极点（pole point）外，
 * 其他十个顶点根据极角 ((+26.565°/-26.565°)（tan^(-1)(0.5)) 和 方位角 72° * n (0 <= n <= 5) 生成
 *
 * @param radius
 * @param subdivide
 * @return Mesh::Ptr
 */
Mesh::Ptr Mesh::CreateSphereMeshIcosahedron(float32 radius, int32 subdivide)
{
    int32 num = 20 * ((subdivide + 1) + (subdivide + 1) * (subdivide ) / 2);
    Mesh::Ptr mesh = std::make_shared<Mesh>();
    mesh->Initialize(num, MeshDataFlag::DEFAULT);

    int32 indice_num = 60 * subdivide * subdivide;
    SubMesh::Ptr submesh = std::make_shared<SubMesh>();
    submesh->Initialize(indice_num);
    mesh->AddSubMesh(submesh);

    glm::vec3 ico_vertice[12] = {};
    uint32 icotriangle_indice[60] = {};
    CreateIconsahedron(ico_vertice, icotriangle_indice, 1.0f);
    // CreateIconsahedron(submesh->GetVerticePos(), submesh->GetIndice(), radius);

    int32 vertice_index = 0;
    int32 indice_index = 0;
    glm::vec3 p0, p1, p2, p, p_helper1, p_helper2;
    glm::vec3 edge_0, edge_1;
    for(int32 i = 0; i < 60; i += 3)
    {
        p0 = ico_vertice[icotriangle_indice[i]];
        p1 = ico_vertice[icotriangle_indice[i + 1]];
        p2 = ico_vertice[icotriangle_indice[i + 2]];

        edge_0 = p1 - p0;
        edge_1 = p2 - p0;

        for (int32 row = 0, row_count = subdivide + 1; row < row_count; row++)
        {
            p_helper1 = p0 + edge_0 * (float32)row / (float32)subdivide;
            p_helper2 = p0 + edge_1 * (float32)row / (float32)subdivide;

            int32 col = 0;
            int32 col_count = row + 1;
            for (; col < col_count; col++)
            {
                if(row == 0)
                {
                    p = p_helper1;
                }
                else
                {
                    p = p_helper1 + (p_helper2 - p_helper1) * (float32)col / (float32)row;
                }
                p = glm::normalize(p);
                p *= radius;

                if(row != 0)
                {
                    if(col != 0)
                    {
                        submesh->_indice[indice_index] = vertice_index + col;
                        submesh->_indice[indice_index + 1] = vertice_index + col - row - 1;
                        submesh->_indice[indice_index + 2] = vertice_index + col - 1;

                        indice_index += 3;

                        if (col != col_count - 1)
                        {
                            submesh->_indice[indice_index] = vertice_index + col;
                            submesh->_indice[indice_index + 1] = vertice_index + col - row;
                            submesh->_indice[indice_index + 2] = vertice_index + col - row - 1;

                            indice_index += 3;
                        }
                    }
                }

                // submesh->_indice[indice_index] = indice_index++;

                mesh->_verticePos[vertice_index + col] = p;
            }

            vertice_index += col_count;
        }
    }

    return mesh;
}

void Mesh::CreateIconsahedron(glm::vec3 *vertice, uint32* indice, float32 radius)
{
    vertice[0] = glm::vec3(0.0f, radius, 0.0f);
    vertice[11] = glm::vec3(0.0f, -radius, 0.0f);

    indice[0] = 0;
    indice[11] = 11;

    float32 pole_angle = acot(0.5f);
    float32 azimuth_step = glm::radians<float32>(72.0f);
    float32 cos_pole_angle = cos(pole_angle);
    float32 sin_pole_angle = sin(pole_angle);

    float32 cos_half_azimuth = cos(-azimuth_step * 0.5f);
    float32 sin_half_azimuth = sin(-azimuth_step * 0.5f);

    float32 x = radius * sin_pole_angle;
    float32 y = radius * cos_pole_angle;
    float32 z = x;
    for(int32 i = 0, j = 5; j < 10; i++, j++)
    {
        float32 cos_azimuth = cos(azimuth_step * i);
        float32 sin_azimuth = sin(azimuth_step * i);

        float32 new_x = x * cos_azimuth;
        float32 new_z = z * sin_azimuth;

        // float32 new_x = x * cos_azimuth + z * sin_azimuth;
        // float32 new_z = -x * sin_azimuth + z * cos_azimuth;

        vertice[i + 1] = glm::vec3(new_x, y, new_z);

        float32 new_x1 = new_x * cos_half_azimuth - new_z * sin_half_azimuth;
        float32 new_z1 = new_x * sin_half_azimuth + new_z * cos_half_azimuth;

        vertice[j + 1] = glm::vec3(new_x1, -y, new_z1);

        // if(j == 5)
        //     vertice[j + 1] = glm::vec3(new_x1, -y, new_z1);
        // else
        //     vertice[16 - j] = glm::vec3(new_x1, -y, new_z1);
    }

    for(int32 i = 0, j = 0; i < 10; i++)
    {
        if (i < 5)
        {
            indice[j] = 0;
            if (i == 4)
                indice[j + 1] = 1;
            else
                indice[j + 1] = i + 2;
            indice[j + 2] = i + 1;

            j += 3;
        }
        else
        {
            indice[j] = 11;
            indice[j + 1] = i + 1;
            if (i == 9)
                indice[j + 2] = 6;
            else
                indice[j + 2] = i + 2;

            j += 3;
        }
    }

    for(int32 i = 0, j = 30; i < 5; i++)
    {
        if(i != 0)
        {
            indice[j] = i + 1;
            indice[j + 1] = i + 6;
            indice[j + 2] = i;

            indice[j + 3] = i + 1;
            if(i != 4)
                indice[j + 4] = i + 7;
            else
                indice[j + 4] = 6;
            indice[j + 5] = i + 6;
        }
        else
        {
            indice[j] = 1;
            indice[j + 1] = 6;
            indice[j + 2] = 5;

            indice[j + 3] = 1;
            indice[j + 4] = 7;
            indice[j + 5] = 6;
        }

        j += 6;
    }
}

Mesh::Ptr Mesh::CreateQuadMesh(float32 x_size, float32 y_size)
{
    Mesh::Ptr mesh = std::make_shared<Mesh>();
    mesh->Initialize(4, MeshDataFlag::DEFAULT);

    SubMesh::Ptr submesh = std::make_shared<SubMesh>();
    submesh->Initialize(6);
    mesh->AddSubMesh(submesh);

    float32 half_x_size = x_size * 0.5f;
    float32 half_y_size = y_size * 0.5f;

    int32 indice_num = 6;
    submesh->_indiceNum = indice_num;
    submesh->_indice = new uint32[indice_num]{};
    submesh->_indice[0] = 0;
    submesh->_indice[1] = 1;
    submesh->_indice[2] = 2;

    submesh->_indice[3] = 0;
    submesh->_indice[4] = 2;
    submesh->_indice[5] = 3;

    mesh->_verticePos[0] = glm::vec3(-half_x_size, half_y_size, 0.0f);
    mesh->_verticePos[1] = glm::vec3(-half_x_size, -half_y_size, 0.0f);
    mesh->_verticePos[2] = glm::vec3(half_x_size, -half_y_size, 0.0f);
    mesh->_verticePos[3] = glm::vec3(half_x_size, half_y_size, 0.0f);

    return mesh;
}

Mesh::Ptr Mesh::CreateCubeMesh(float32 size)
{
    Mesh::Ptr mesh = std::make_shared<Mesh>();
    mesh->Initialize(8, MeshDataFlag::DEFAULT);

    SubMesh::Ptr submesh = std::make_shared<SubMesh>();
    submesh->Initialize(36);
    mesh->AddSubMesh(submesh);
    
    int32 index = 0;
    //front
    submesh->_indice[index++] = 0;
    submesh->_indice[index++] = 1;
    submesh->_indice[index++] = 2;

    submesh->_indice[index++] = 0;
    submesh->_indice[index++] = 2;
    submesh->_indice[index++] = 3;

    //left
    submesh->_indice[index++] = 4;
    submesh->_indice[index++] = 5;
    submesh->_indice[index++] = 1;

    submesh->_indice[index++] = 4;
    submesh->_indice[index++] = 1;
    submesh->_indice[index++] = 0;

    //right
    submesh->_indice[index++] = 3;
    submesh->_indice[index++] = 2;
    submesh->_indice[index++] = 6;

    submesh->_indice[index++] = 3;
    submesh->_indice[index++] = 6;
    submesh->_indice[index++] = 7;

    //back
    submesh->_indice[index++] = 7;
    submesh->_indice[index++] = 6;
    submesh->_indice[index++] = 5;

    submesh->_indice[index++] = 7;
    submesh->_indice[index++] = 5;
    submesh->_indice[index++] = 4;

    //top
    submesh->_indice[index++] = 4;
    submesh->_indice[index++] = 0;
    submesh->_indice[index++] = 3;

    submesh->_indice[index++] = 4;
    submesh->_indice[index++] = 3;
    submesh->_indice[index++] = 7;

    //bottom
    submesh->_indice[index++] = 1;
    submesh->_indice[index++] = 5;
    submesh->_indice[index++] = 6;

    submesh->_indice[index++] = 1;
    submesh->_indice[index++] = 6;
    submesh->_indice[index++] = 2;

    float32 half_x_size = size * 0.5f;
    float32 half_y_size = size * 0.5f;
    float32 half_z_size = size * 0.5f;

    mesh->_verticePos[0] = glm::vec3(-half_x_size, half_y_size, half_z_size);
    mesh->_verticePos[1] = glm::vec3(-half_x_size, -half_y_size, half_z_size);
    mesh->_verticePos[2] = glm::vec3(half_x_size, -half_y_size, half_z_size);
    mesh->_verticePos[3] = glm::vec3(half_x_size, half_y_size, half_z_size);

    mesh->_verticePos[4] = glm::vec3(-half_x_size, half_y_size, -half_z_size);
    mesh->_verticePos[5] = glm::vec3(-half_x_size, -half_y_size, -half_z_size);
    mesh->_verticePos[6] = glm::vec3(half_x_size, -half_y_size, -half_z_size);
    mesh->_verticePos[7] = glm::vec3(half_x_size, half_y_size, -half_z_size);

    return mesh;
}


/**
 * @brief use geometry shader render line mesh
 * 
 * @param points 
 * @param num 
 * @return Mesh::Ptr 
 */
Mesh::Ptr Mesh::CreateLineMesh(glm::vec3 *points, int32 num)
{
    Mesh::Ptr mesh = std::make_shared<Mesh>();
    mesh->Initialize(num, MeshDataFlag::DEFAULT);

    //use line_adjency
    int32 indice_num = (num - 1) * 4;
    SubMesh::Ptr submesh = std::make_shared<SubMesh>();
    submesh->Initialize(indice_num);
    mesh->AddSubMesh(submesh);

    int32 indice_index = 0;
    for(int32 i = 0; i < num - 1; i++)
    {
        if(i == 0)
        {
            submesh->_indice[indice_index++] = 0;
        }
        else
        {
            submesh->_indice[indice_index++] = i - 1;
        }

        submesh->_indice[indice_index++] = i;
        submesh->_indice[indice_index++] = i + 1;

        if(i == num - 2)
        {
            submesh->_indice[indice_index++] = num - 1;
        }
        else
        {
            submesh->_indice[indice_index++] = i + 2;
        }
    }

    if(points != nullptr)
    {
        for (int32 i = 0; i < num; i++)
        {
            mesh->_verticePos[i] = points[i];
        }
    }

    return mesh;
}

Mesh::Ptr Mesh::CreateLineMeshEx(glm::vec4 *points, int32 num)
{
    Mesh::Ptr mesh = std::make_shared<Mesh>();
    mesh->Initialize(num, MeshDataFlag::DEFAULT);

    //use line_adjency
    int32 indice_num = (num - 1) * 4;
    SubMesh::Ptr submesh = std::make_shared<SubMesh>();
    submesh->Initialize(indice_num);
    mesh->AddSubMesh(submesh);

    int32 indice_index = 0;
    for(int32 i = 0; i < num - 1; i++)
    {
        if(i == 0)
        {
            submesh->_indice[indice_index++] = 0;
        }
        else
        {
            submesh->_indice[indice_index++] = i - 1;
        }

        submesh->_indice[indice_index++] = i;
        submesh->_indice[indice_index++] = i + 1;

        if(i == num - 2)
        {
            submesh->_indice[indice_index++] = num - 1;
        }
        else
        {
            submesh->_indice[indice_index++] = i + 2;
        }
    }

    if(points != nullptr)
    {
        for (int32 i = 0; i < num; i++)
        {
            mesh->_verticePos[i] = glm::vec3(points[i]);
        }
    }

    return mesh;
}

Mesh::Ptr Mesh::CreateBezierLine(glm::vec3 *points, int32 num, int32 segments)
{
    Mesh::Ptr mesh = CreateLineMesh(nullptr, segments + 1);

    SubMesh::Ptr submesh = mesh->GetSubMesh(0);
    glm::vec3 *vertices = mesh->GetVerticePos();
    vertices[0] = points[0];
    vertices[segments] = points[num - 1];

    float32 u_step = 1.0f / (float32)segments;

    glm::vec3 *points_helper = new glm::vec3[num];
    for (int32 i = 1; i < segments; i++)
    {
        // memcpy((void *)points_helper, (void *)points, sizeof(glm::vec3) * num);
        vertices[i] = DeCasteljau(points, points_helper, num, u_step * i);

        // std::cout << "------------------------" << std::endl;
        // std::cout << "x:" << results[i].x << std::endl;
        // std::cout << "y:" << results[i].y << std::endl;
        // std::cout << "z:" << results[i].z << std::endl;
    }

    SAFE_DEL_ARR(points_helper);

    return mesh;
}

/**
 * @brief (refer http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/)
 * 
 * @param points 
 * @param num 
 * @param segments 
 * @return Mesh::Ptr 
 */
Mesh::Ptr Mesh::CreateQuadraticBezierLine(glm::vec3 *points, int32 segments)
{
    return CreateBezierLine(points, 3, segments);
}

/**
 * @brief (refer http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/)
 * 
 * @param points 
 * @param num 
 * @param segments 
 * @return Mesh::Ptr 
 */
Mesh::Ptr Mesh::CreateCubicBezierLine(glm::vec3 *points, int32 segments)
{
    return CreateBezierLine(points, 4, segments);
}

/**
 * @brief (refer http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/)
 * 
 * @param points 
 * @param num 
 * @param u 
 * @return glm::vec3 
 */
glm::vec3 Mesh::DeCasteljau(glm::vec3 *points, glm::vec3* helper, int32 num, float32 u)
{
    int32 point_count = num - 1;
    for(int32 i = 0, iteration = num - 1; i < iteration; i++)
    {
        for(int32 j = 0; j < point_count; j++)
        {
            if(i == 0)
                helper[j] = (1.0f - u) * points[j] + u * points[j + 1];
            else
            {
                helper[j] = (1.0f - u) * helper[j] + u * helper[j + 1];
            }        
        }
        --point_count;
    }

    return helper[0];
}

} // namespace TwinkleGraphics
