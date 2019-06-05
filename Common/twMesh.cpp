
#include "twMesh.h"

namespace TwinkleGraphics
{


void SubMesh::Initialize(int32 num, MeshDataFlag flag)
{
    if(num <= 0) return;

    _data_flag = flag;
    _vertice_pos = new glm::vec3[num];
    _vertice_num = num;

    if(flag == MeshDataFlag::DEFAULT) return;

    if((flag & MeshDataFlag::HAS_COLOR) != 0)
    {
        _vertice_color = new glm::vec4[num];
    }

    if((flag & MeshDataFlag::HAS_BINORMAL) != 0)
    {
        _vertice_binormal = new glm::vec3[num];
    }

    if((flag & MeshDataFlag::HAS_NORMAL) != 0)
    {
        _vertice_normal = new glm::vec3[num];
    }

    if((flag & MeshDataFlag::HAS_UV) != 0)
    {
        _vertice_uv = new glm::vec4[num];
    }
}









Mesh::Ptr Mesh::CreateQuadMesh(float32 x, float32 y)
{
    Mesh::Ptr mesh = std::make_shared<Mesh>();

    return mesh;
}

Mesh::Ptr Mesh::CreateCubeMesh(float32 size)
{
    Mesh::Ptr mesh = std::make_shared<Mesh>();

    return mesh;
}

/**
 * @brief 创建球面网格的基础算法（https://zh.wikipedia.org/wiki/%E7%90%83%E5%BA%A7%E6%A8%99%E7%B3%BB）
 * 球坐标系由点 p 到原点 o 的距离 r，极角（pole angle）和方位角（azimuth angle）构成，若已知这三个变量，
 * 可通过球坐标系转直角坐标系求得其直角坐标（x,y,z）
 * @param radius 
 * @param longtitude_step 
 * @param lantitude_step 
 * @return Mesh::Ptr 
 */
Mesh::Ptr Mesh::CreateSphereMeshStandard(float32 radius, int32 longitude_count, int32 latitude_count)
{
    SubMesh::Ptr submesh = std::make_shared<SubMesh>();

    int32 row_count = latitude_count - 1;
    int32 col_count = longitude_count;
    int32 num = col_count * row_count + 2;
    
    submesh->Initialize(num, MeshDataFlag::DEFAULT);

    float32 u_step = glm::two_pi<float32>() / (float32)longitude_count;
    float32 v_step = glm::pi<float32>() / (float32)latitude_count;

    //pole points
    submesh->_vertice_pos[0].x = 0.0f;
    submesh->_vertice_pos[0].y = radius;
    submesh->_vertice_pos[0].z = 0.0f;

    submesh->_vertice_pos[num - 1].x = 0.0f;
    submesh->_vertice_pos[num - 1].y = -radius;
    submesh->_vertice_pos[num - 1].z = 0.0f;

    //compute vertice position
    float32 x, y, z;
    for(int32 i = 0, k = 1; i < row_count; i++)
    {
        y = radius * cos(v_step * (i + 1));
        for(int32 j = 0; j < col_count; j++)
        {
            x = radius * sin(v_step * (i + 1)) * cos(u_step * j);
            z = radius * sin(v_step * (i + 1)) * sin(u_step * j);

            submesh->_vertice_pos[k++] = glm::vec3(x, y, z);
            // submesh->_vertice_pos[k].x = x;
            // submesh->_vertice_pos[k].y = y;
            // submesh->_vertice_pos[k++].z = z;
        }
    }

    // submesh->_indice_num = num;
    // submesh->_indice = new uint32[num];
    // for(int i = 0; i < num; i++)
    // {
    //     submesh->_indice[i] = i;
    // }

    //compute indice
    int indice_num = 6 * col_count + (row_count - 1) * col_count * 6;
    submesh->_indice_num = indice_num;
    submesh->_indice = new uint32[indice_num];

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

    Mesh::Ptr mesh = std::make_shared<Mesh>();
    mesh->AddSubMesh(submesh);

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
    SubMesh::Ptr submesh = std::make_shared<SubMesh>();

    int32 row_count, col_count;
    row_count = col_count = subdivide + 1;
    int32 num = row_count * col_count * 6;

    submesh->Initialize(num, MeshDataFlag::DEFAULT);

    submesh->_indice_num = subdivide * subdivide * 36;
    submesh->_indice = new uint32[submesh->_indice_num];

    // submesh->_indice_num = num;
    // submesh->_indice = new uint32[num];

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
    int32 vertex_index = 0;
    int32 element_index = 0;
    int32 face_index = -1;
    float32 step = 1.0f / subdivide;
    for(int32 i = 0; i < 24; i += 4)
    {
        face_index = i / 4;
        vertex_index = face_index * row_count * col_count;
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

                // submesh->_indice[element_index++] = vertex_index;

                if(j != row_count - 1 && k != col_count - 1)
                {
                    submesh->_indice[element_index] = vertex_index;
                    submesh->_indice[element_index + 1] = vertex_index + col_count;
                    submesh->_indice[element_index + 2] = vertex_index + col_count + 1;

                    submesh->_indice[element_index + 3] = vertex_index;
                    submesh->_indice[element_index + 4] = vertex_index + col_count + 1;
                    submesh->_indice[element_index + 5] = vertex_index + 1;

                    element_index += 6;
                }

                submesh->_vertice_pos[vertex_index++] = coord;
            }
        }
    }

    Mesh::Ptr mesh = std::make_shared<Mesh>();
    mesh->AddSubMesh(submesh);

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
    glm::vec3 ico_vertice[12] = {};
    uint32 icotriangle_indice[60] = {};
    CreateIconsahedron(ico_vertice, icotriangle_indice);
    
}

void Mesh::CreateIconsahedron(glm::vec3 *vertice, uint32* indice)
{
    vertice[0] = glm::vec3(0.0f, 1.0f, 0.0f);
    vertice[11] = glm::vec3(0.0f, -1.0f, 0.0f);

    float32 azimuth_angle = glm::radians<float32>(72);

    float32 x, y, z;
    float32 pole_angle = atan(0.5f);
    for(int i = 0, j = 0; i < 10; i++)
    {
        if(i >= 5)
            pole_angle = -atan(0.5f);
        y = cos(pole_angle);

        int n = i;
        if(i >= 5)
        {
            n = i - 5;
        }
        x = sin(pole_angle) * cos(azimuth_angle * n);
        z = sin(pole_angle) * sin(azimuth_angle * n);

        vertice[i + 1] = glm::vec3(x, y, z);

        if(i < 5)
        {
            indice[j] = 0;
            indice[j + 1] = i + 1;
            indice[j + 2] = i + 2;

            j += 3;
        }
        else
        {
            indice[j] = 11;
            indice[j + 1] = i + 1;
            indice[j + 2] = i + 2;

            j += 3;
        }
    }

    
}

} // namespace TwinkleGraphics
