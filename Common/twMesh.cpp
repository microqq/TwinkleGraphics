
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
Mesh::Ptr Mesh::CreateSphereMeshStandard(float32 radius, int32 longtitude_count, int32 lantitude_count)
{
    SubMesh::Ptr submesh = std::make_shared<SubMesh>();

    int32 row_count = lantitude_count - 1;
    int32 col_count = longtitude_count;
    int32 num = col_count * row_count + 2;
    
    submesh->Initialize(num, MeshDataFlag::DEFAULT);

    float32 u_step = glm::two_pi<float32>() / (float32)longtitude_count;
    float32 v_step = glm::pi<float32>() / (float32)lantitude_count;

    //pole points
    submesh->_vertice_pos[0].x = 0.0f;
    submesh->_vertice_pos[0].y = radius;
    submesh->_vertice_pos[0].z = 0.0f;

    submesh->_vertice_pos[num - 1].x = 0.0f;
    submesh->_vertice_pos[num - 1].y = -radius;
    submesh->_vertice_pos[num - 1].z = 0.0f;

    float32 x, y, z;
    for(int32 i = 0, k = 1; i < row_count; i++)
    {
        y = radius * cos(v_step * (i + 1));
        for(int32 j = 0; j < col_count; j++)
        {
            x = radius * sin(v_step * (i + 1)) * cos(u_step * j);
            z = radius * sin(v_step * (i + 1)) * sin(u_step * j);

            //int32 index = 1 + i * col_count + j;
            // submesh->_vertice_pos[k++] = glm::vec3(x, y, z);
            submesh->_vertice_pos[k].x = x;
            submesh->_vertice_pos[k].y = y;
            submesh->_vertice_pos[k++].z = z;
        }
    }

    // submesh->_indice_num = num;
    // submesh->_indice = new uint32[num];
    // for(int i = 0; i < num; i++)
    // {
    //     submesh->_indice[i] = i;
    // }

    int indice_num = 6 * col_count + (row_count - 1) * col_count * 6;
    submesh->_indice_num = indice_num;
    submesh->_indice = new uint32[indice_num];

    for(int32 i = 1, j = 0; i < num - 1; i++)
    {
        int32 row = (i - 1) / col_count;
        int32 col = (i - 1) % col_count;
        if(0 == row)
        {
            if(col == col_count - 1)
            {
                submesh->_indice[j] = row * col_count + col;
                submesh->_indice[j + 1] = row * col_count;
                submesh->_indice[j + 2] = 0;
                
                j += 3;
            }
            else
            {
                submesh->_indice[j] = row * col_count + col;
                submesh->_indice[j + 1] = row * col_count + col + 1;
                submesh->_indice[j + 2] = 0;

                j += 3;
            }            
        }
        else
        {
            int32 topRow = row - 1;
            if(col == col_count - 1)
            {
                submesh->_indice[j] = row * col_count + col;
                submesh->_indice[j + 1] = topRow * col_count;
                submesh->_indice[j + 2] = topRow * col_count;

                submesh->_indice[j + 3] = row * col_count + col;
                submesh->_indice[j + 4] = row * col_count;
                submesh->_indice[j + 5] = topRow * col_count;
            }
            else
            {
                submesh->_indice[j] = row * col_count + col;
                submesh->_indice[j + 1] = topRow * col_count + col + 1;
                submesh->_indice[j + 2] = topRow * col_count + col;

                submesh->_indice[j + 3] = row * col_count + col;
                submesh->_indice[j + 4] = row * col_count + col + 1;
                submesh->_indice[j + 5] = topRow * col_count + col + 1;
            }

            j += 6;

            if(row == row_count - 1)
            {
                if(col == col_count - 1)
                {
                    submesh->_indice[j] = row * col_count + col;
                    submesh->_indice[j + 1] = num - 1;
                    submesh->_indice[j + 2] = row * col_count;

                    j += 3;
                }
                else
                {
                    submesh->_indice[j] = row * col_count + col;
                    submesh->_indice[j + 1] = num - 1;
                    submesh->_indice[j + 2] = row * col_count + col + 1;

                    j += 3;
                }
            }
        }
    }

    Mesh::Ptr mesh = std::make_shared<Mesh>();
    mesh->AddSubMesh(submesh);

    return mesh;
}

/**
 * @brief 单位立方体细分法
 * 根据细分程度递归细分立方体六个面，新的顶点归一化，最后将得到顶点坐标乘以球半径 r
 * @param radius 
 * @param subdivide 
 * @return Mesh::Ptr 
 */
Mesh::Ptr Mesh::CreateSphereMeshNormalizedCube(float32 radius, int32 subdivide)
{
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
 * @param radius 
 * @param subdivide 
 * @return Mesh::Ptr 
 */
Mesh::Ptr Mesh::CreateSphereMeshIcosahedron(float radius, int subdivide)
{
}


} // namespace TwinkleGraphics
