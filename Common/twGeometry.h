

#ifndef TW_GEOMETRY_H
#define TW_GEOMETRY_H

#include "twCommon.h"
#include "twMesh.h"
#include "twMeshRenderer.h"
#include "twTransform.h"
#include "twSceneNode.h"

namespace TwinkleGraphics
{
/**
 * @brief 
 * 
 */
class Geometry : public Object, public ISceneNode, public IRenderableObject
{
public:
    typedef std::shared_ptr<Geometry> Ptr;

    Geometry();
    virtual ~Geometry();

    virtual void GenerateMesh() {}
    Mesh::Ptr GetMesh() { return _mesh; }
    
    MeshDataFlag GetMeshDataFlag() { return _flag; }

protected:
    Mesh::Ptr _mesh;
    MeshDataFlag _flag;
};

class Triangle : public Geometry
{
public: 
    typedef std::shared_ptr<Triangle> Ptr;

    Triangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, MeshDataFlag flag = MeshDataFlag::DEFAULT)
        : Geometry()
        , _p0(p0)
        , _p1(p1)
        , _p2(p2)
    {
        _flag = flag;
    }
    virtual ~Triangle()
    {}

    virtual void GenerateMesh() override
    {
        if(_mesh == nullptr)
        {
            SubMesh::Ptr submesh = std::make_shared<SubMesh>();
            submesh->Initialize(3, 0, _flag);

            submesh->_indice_num = 3;
            submesh->_indice = new uint32[3];

            submesh->_vertice_pos[0] = _p0;
            submesh->_vertice_pos[1] = _p1;
            submesh->_vertice_pos[2] = _p2;

            submesh->_indice[0] = 0;
            submesh->_indice[1] = 1;
            submesh->_indice[2] = 2;

            _mesh = std::make_shared<Mesh>();
            _mesh->AddSubMesh(submesh);
        }
    }

private:
    glm::vec3 _p0;
    glm::vec3 _p1;
    glm::vec3 _p2;
};

/**
 * @brief 
 * 
 */
class UVSphere : public Geometry
{
public: 
    typedef std::shared_ptr<UVSphere> Ptr;

    UVSphere(float32 radius = 1.0f, int32 subdivision = 20, MeshDataFlag flag = MeshDataFlag::DEFAULT)
        : Geometry()
        , _radius(radius)
        , _subdivision(subdivision)
    {
        _flag = flag;
    }
    virtual ~UVSphere()
    {}

    virtual void GenerateMesh() override
    {
        if(_mesh == nullptr)
        {
            _mesh = CreateSphereMeshStandard(_radius, _subdivision, _subdivision);
        }
    }

private:
    /**
 * @brief 创建球面网格的基础算法（https://zh.wikipedia.org/wiki/%E7%90%83%E5%BA%A7%E6%A8%99%E7%B3%BB）
 * 球坐标系由点 p 到原点 o 的距离 r，极角（pole angle）和方位角（azimuth angle）构成，若已知这三个变量，
 * 可通过球坐标系转直角坐标系求得其直角坐标（x,y,z）
 * @param radius
 * @param longitude_count
 * @param latitude_count
 * @return Mesh::Ptr
 */
    Mesh::Ptr CreateSphereMeshStandard(float32 radius, int32 longitude_count, int32 latitude_count)
    {
        SubMesh::Ptr submesh = std::make_shared<SubMesh>();

        int32 row_count = latitude_count - 1;
        int32 col_count = longitude_count;
        int32 num = col_count * row_count + 2;

        submesh->Initialize(num, 0, _flag);

        float32 u_step = glm::two_pi<float32>() / (float32)(longitude_count - 1);
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
        for (int32 i = 0, k = 1; i < row_count; i++)
        {
            y = radius * cos(v_step * (i + 1));
            for (int32 j = 0; j < col_count; j++)
            {
                x = radius * sin(v_step * (i + 1)) * cos(u_step * j);
                z = radius * sin(v_step * (i + 1)) * sin(u_step * j);

                submesh->_vertice_pos[k++] = glm::vec3(x, y, z);
            }
        }

        //compute indice
        SAFE_DEL_ARR(submesh->_indice);
        int indice_num = 6 * col_count + (row_count - 1) * col_count * 6;
        submesh->_indice_num = indice_num;
        submesh->_indice = new uint32[indice_num];

        for (int32 i = 0, j = 0; i < num - 2; i++)
        {
            int32 row = i / col_count;
            int32 col = i % col_count;
            if (0 == row)
            {
                if (col == col_count - 1)
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
                if (col == col_count - 1)
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

                if (row == row_count - 1)
                {
                    if (col == col_count - 1)
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

private:
    float32 _radius;
    int32 _subdivision;
};

class NormalizedCubeSphere : public Geometry
{
public:
    typedef std::shared_ptr<NormalizedCubeSphere> Ptr;

    NormalizedCubeSphere(float32 radius = 1.0f, int32 subdivision = 20, MeshDataFlag flag = MeshDataFlag::DEFAULT)
        : Geometry()
        , _radius(radius)
        , _subdivision(subdivision)
    {
        _flag = flag;
    }
    virtual ~NormalizedCubeSphere()
    {}

    virtual void GenerateMesh() override
    {
        if(_mesh == nullptr)
        {
            _mesh = CreateSphereMeshNormalizedCube(_radius, _subdivision);
        }
    }

private:
    /**
     * @brief 单位立方体均匀细分法（uniform subdive normalized cube）
     * 根据细分程度递归细分立方体六个面，新的顶点归一化，最后将得到顶点坐标乘以球半径 r
     *
     * @param radius
     * @param subdivide
     * @return Mesh::Ptr
     */
    Mesh::Ptr CreateSphereMeshNormalizedCube(float32 radius, int32 subdivide)
    {
        SubMesh::Ptr submesh = std::make_shared<SubMesh>();

        int32 row_count, col_count;
        row_count = col_count = subdivide + 1;
        int32 num = row_count * col_count * 6;

        submesh->Initialize(num, 0, _flag);

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
        int32 vertice_index = 0;
        int32 element_index = 0;
        int32 face_index = -1;
        float32 step = 1.0f / subdivide;
        for (int32 i = 0; i < 24; i += 4)
        {
            face_index = i / 4;
            vertice_index = face_index * row_count * col_count;
            for (int32 j = 0; j < row_count; j++)
            {
                for (int32 k = 0; k < col_count; k++)
                {
                    //double linear interpolation
                    glm::vec3 col_temp_1 = (faces[i + 3] - faces[i]) * (float32)k * step + faces[i];
                    glm::vec3 col_temp_2 = (faces[i + 2] - faces[i + 1]) * (float32)k * step + faces[i + 1];
                    glm::vec3 coord = (col_temp_2 - col_temp_1) * (float32)j * step + col_temp_1;
                    coord = glm::normalize(coord);
                    coord *= radius;

                    // submesh->_indice[element_index++] = vertice_index;

                    if (j != row_count - 1 && k != col_count - 1)
                    {
                        submesh->_indice[element_index] = vertice_index;
                        submesh->_indice[element_index + 1] = vertice_index + col_count;
                        submesh->_indice[element_index + 2] = vertice_index + col_count + 1;

                        submesh->_indice[element_index + 3] = vertice_index;
                        submesh->_indice[element_index + 4] = vertice_index + col_count + 1;
                        submesh->_indice[element_index + 5] = vertice_index + 1;

                        element_index += 6;
                    }

                    submesh->_vertice_pos[vertice_index++] = coord;
                }
            }
        }

        Mesh::Ptr mesh = std::make_shared<Mesh>();
        mesh->AddSubMesh(submesh);

        return mesh;
    }

private:
    float32 _radius;
    int32 _subdivision;
};

class IcosahedronSphere : public Geometry
{
public:
    typedef std::shared_ptr<IcosahedronSphere> Ptr;

    IcosahedronSphere(float32 radius = 1.0f, int32 subdivision = 20, MeshDataFlag flag = MeshDataFlag::DEFAULT)
        : Geometry()
        , _radius(radius)
        , _subdivision(subdivision)
    {
        _flag = flag;
    }
    virtual ~IcosahedronSphere()
    {}

    virtual void GenerateMesh() override
    {
        if(_mesh == nullptr)
        {
            _mesh = CreateSphereMeshIcosahedron(_radius, _subdivision);
        }
    }

private:
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
    Mesh::Ptr CreateSphereMeshIcosahedron(float32 radius, int32 subdivide)
    {
        SubMesh::Ptr submesh = std::make_shared<SubMesh>();

        int32 num = 20 * ((subdivide + 1) + (subdivide + 1) * (subdivide) / 2);
        submesh->Initialize(num, 0, _flag);

        int32 indice_num = 60 * subdivide * subdivide;
        submesh->_indice_num = indice_num;
        submesh->_indice = new uint32[indice_num];

        // submesh->_indice_num = 60;
        // submesh->_indice = new uint32[60];

        glm::vec3 ico_vertice[12] = {};
        uint32 icotriangle_indice[60] = {};
        CreateIconsahedron(ico_vertice, icotriangle_indice, 1.0f);
        // CreateIconsahedron(submesh->GetVerticePos(), submesh->GetIndice(), radius);

        int32 vertice_index = 0;
        int32 indice_index = 0;
        glm::vec3 p0, p1, p2, p, p_helper1, p_helper2;
        glm::vec3 edge_0, edge_1;
        for (int32 i = 0; i < 60; i += 3)
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
                    if (row == 0)
                    {
                        p = p_helper1;
                    }
                    else
                    {
                        p = p_helper1 + (p_helper2 - p_helper1) * (float32)col / (float32)row;
                    }
                    p = glm::normalize(p);
                    p *= radius;

                    if (row != 0)
                    {
                        if (col != 0)
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

                    submesh->_vertice_pos[vertice_index + col] = p;
                }

                vertice_index += col_count;
            }
        }

        Mesh::Ptr mesh = std::make_shared<Mesh>();
        mesh->AddSubMesh(submesh);

        return mesh;
    }

    void CreateIconsahedron(glm::vec3 *vertice, uint32 *indice, float32 radius)
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
        for (int32 i = 0, j = 5; j < 10; i++, j++)
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

        for (int32 i = 0, j = 0; i < 10; i++)
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

        for (int32 i = 0, j = 30; i < 5; i++)
        {
            if (i != 0)
            {
                indice[j] = i + 1;
                indice[j + 1] = i + 6;
                indice[j + 2] = i;

                indice[j + 3] = i + 1;
                if (i != 4)
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

private:
    float32 _radius;
    int32 _subdivision;
};

class Cube : public Geometry
{
public:
    typedef std::shared_ptr<Cube> Ptr;

    Cube(float32 size = 1.0f, MeshDataFlag flag = MeshDataFlag::DEFAULT)
        : Geometry()
        , _size(size)
    {
        _flag = flag;
    }
    virtual ~Cube()
    {}

    virtual void GenerateMesh() override
    {
        if(_mesh == nullptr)
        {
            _mesh = CreateCubeMesh(_size);
        }
    }

private:
    Mesh::Ptr CreateCubeMesh(float32 size)
    {
        SubMesh::Ptr submesh = std::make_shared<SubMesh>();
        submesh->Initialize(8, 0, _flag);

        int32 indice_num = 36;
        submesh->_indice_num = indice_num;
        submesh->_indice = new uint32[indice_num];

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

        submesh->_vertice_pos[0] = glm::vec3(-half_x_size, half_y_size, half_z_size);
        submesh->_vertice_pos[1] = glm::vec3(-half_x_size, -half_y_size, half_z_size);
        submesh->_vertice_pos[2] = glm::vec3(half_x_size, -half_y_size, half_z_size);
        submesh->_vertice_pos[3] = glm::vec3(half_x_size, half_y_size, half_z_size);

        submesh->_vertice_pos[4] = glm::vec3(-half_x_size, half_y_size, -half_z_size);
        submesh->_vertice_pos[5] = glm::vec3(-half_x_size, -half_y_size, -half_z_size);
        submesh->_vertice_pos[6] = glm::vec3(half_x_size, -half_y_size, -half_z_size);
        submesh->_vertice_pos[7] = glm::vec3(half_x_size, half_y_size, -half_z_size);

        Mesh::Ptr mesh = std::make_shared<Mesh>();
        mesh->AddSubMesh(submesh);
        return mesh;
    }

private:
    float32 _size;
};

class Quad : public Geometry
{
public:
    typedef std::shared_ptr<Quad> Ptr;

    Quad(MeshDataFlag flag = MeshDataFlag::DEFAULT)
        : Geometry()
        , _size()
    {
        _flag = flag;
    }
    Quad(glm::vec2 size, MeshDataFlag flag = MeshDataFlag::DEFAULT)
        : Geometry()
        , _size(size)
    {
        _flag = flag;
    }
    virtual ~Quad()
    {}

    void SetSize(glm::vec2 size) { _size = size; }
    virtual void GenerateMesh() override
    {
        if(_mesh == nullptr)
        {
            _mesh = CreateQuadMesh(_size.x, _size.y);
        }
    }

protected:
    Mesh::Ptr CreateQuadMesh(float32 x_size, float32 y_size)
    {
        SubMesh::Ptr submesh = std::make_shared<SubMesh>();
        submesh->Initialize(4, 0, _flag);

        float32 half_x_size = x_size * 0.5f;
        float32 half_y_size = y_size * 0.5f;

        int32 indice_num = 6;
        submesh->_indice_num = indice_num;
        submesh->_indice = new uint32[indice_num]{};
        submesh->_indice[0] = 0;
        submesh->_indice[1] = 1;
        submesh->_indice[2] = 2;

        submesh->_indice[3] = 0;
        submesh->_indice[4] = 2;
        submesh->_indice[5] = 3;

        /**
         * @brief
         * 0 __ __ __ 3
         *  |        |
         *  |        |
         *  |__ __ __|
         * 1          2
         */

        submesh->_vertice_pos[0] = glm::vec3(-half_x_size, half_y_size, 0.0f);
        submesh->_vertice_pos[1] = glm::vec3(-half_x_size, -half_y_size, 0.0f);
        submesh->_vertice_pos[2] = glm::vec3(half_x_size, -half_y_size, 0.0f);
        submesh->_vertice_pos[3] = glm::vec3(half_x_size, half_y_size, 0.0f);

        if((_flag & MeshDataFlag::HAS_UV) != 0)
        {
            glm::vec4* uvs = submesh->GetVerticeUV();

            /**
             * @brief
             * 0 __ __ __ 3
             *  |        |
             *  |        |
             *  |__ __ __|
             * 1          2
             */
            
            uvs[0] = glm::vec4(0.0f, 1.0f, 0.0, 0.0f);
            uvs[1] = glm::vec4(0.0f, 0.0f, 0.0, 0.0f);
            uvs[2] = glm::vec4(1.0f, 0.0f, 0.0, 0.0f);
            uvs[3] = glm::vec4(1.0f, 1.0f, 0.0, 0.0f);
        }

        Mesh::Ptr mesh = std::make_shared<Mesh>();
        mesh->AddSubMesh(submesh);
        return mesh;
    }

protected:
    glm::vec2 _size;
};


class Plane : public Geometry
{
public:
    typedef std::shared_ptr<Plane> Ptr;

    Plane(glm::vec3 normal, float32 width, int32 subdivision = 32, MeshDataFlag flag = MeshDataFlag::DEFAULT)
        : Geometry()
        , _normal(normal)
        , _width(width)
        , _subdivision(subdivision)
    {
        _flag = flag;
    }
    virtual ~Plane()
    {}

    virtual void GenerateMesh() override
    {
        if(_mesh == nullptr)
        {
            _mesh = CreatePlaneMesh();
        }
    }

protected:
    Mesh::Ptr CreatePlaneMesh()
    {
        int32 row_count = _subdivision + 1;
        int32 col_count = _subdivision + 1;
        SubMesh::Ptr submesh = std::make_shared<SubMesh>();
        submesh->Initialize(row_count * col_count, 0, _flag);

        int32 indice_num = _subdivision * _subdivision * 6;
        submesh->_indice_num = indice_num;
        submesh->_indice = new uint32[indice_num]{};

        int32 indice_index = 0;
        for(int32 i = 0; i < _subdivision; i++)
        {
            for (int32 j = 0; j < _subdivision; j++)
            {
                submesh->_indice[indice_index] = i * col_count + j;
                submesh->_indice[indice_index + 1] = i * col_count + col_count + j;
                submesh->_indice[indice_index + 2] = i * col_count + col_count + j + 1;

                submesh->_indice[indice_index + 3] = i * col_count + j;
                submesh->_indice[indice_index + 4] = i * col_count + col_count + j + 1;
                submesh->_indice[indice_index + 5] = i * col_count + j + 1;

                indice_index += 6;
            }
        }

        /**
         * @brief
         * 0 __ __ __ 3
         *  |        |
         *  |        |
         *  |__ __ __|
         * 1          2
         */
        glm::vec4* uvs = submesh->GetVerticeUV();
        glm::vec3 axis = glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), _normal));
        float32 cos_theta = glm::dot(_normal, glm::vec3(0.0f, 0.0f, 1.0f));
        float32 theta = ::acosf(cos_theta);
        glm::mat3 mat_rot = glm::mat3_cast(
            glm::quat(::sinf(theta * 0.5f), ::cosf(theta * 0.5f) * axis)
        );
        if(_normal == glm::vec3(0.0f, 0.0f, 1.0f))
        {
            mat_rot = glm::identity<glm::mat3>();
        }

        for(int32 i = 0; i < row_count; i++)
        {
            float32 y = _width * 0.5f - (_width / _subdivision) * i;
            float32 v = 1.0f - (1.0f / _subdivision) * i;

            for (int32 j = 0; j < col_count; j++)
            {
                float32 x = (_width / _subdivision) * j - _width * 0.5f;
                float32 u = (1.0f / _subdivision) * j - 1.0f;

                glm::vec3 point(x, y, 0.0f);
                submesh->_vertice_pos[i * col_count + j] = mat_rot * point;

                if ((_flag & MeshDataFlag::HAS_UV) != 0)
                {
                    uvs[0] = glm::vec4(u, v, 0.0, 0.0f);
                }
            }
        }

        Mesh::Ptr mesh = std::make_shared<Mesh>();
        mesh->AddSubMesh(submesh);
        return mesh;
    }

protected:
    glm::vec3 _normal;
    float32 _width;
    int32 _subdivision;
};

class Line : public Geometry
{
public:
    typedef std::shared_ptr<Line> Ptr;

    Line(glm::vec3 *points, int32 num)
        : Geometry()
    {
        Generate(points, num);
    }
    virtual ~Line()
    {}

    void Generate(glm::vec3 *points, int32 num)
    {
        if(_mesh == nullptr)
        {
            _mesh = CreateLineMesh(points, num);
        }
    }

private:
    /**
     * @brief use geometry shader render line mesh
     * 
     * @param points 
     * @param num 
     * @return Mesh::Ptr 
     */
    Mesh::Ptr CreateLineMesh(glm::vec3 *points, int32 num)
    {
        SubMesh::Ptr submesh = std::make_shared<SubMesh>();
        submesh->Initialize(num, 0, MeshDataFlag::DEFAULT);

        //use line_adjency
        int32 indice_num = (num - 1) * 4;
        submesh->_indice_num = indice_num;
        submesh->_indice = new uint32[indice_num];

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
                submesh->_vertice_pos[i] = points[i];
            }
        }

        Mesh::Ptr mesh = std::make_shared<Mesh>();
        mesh->AddSubMesh(submesh);

        return mesh;
    }

private:
};

/**
 * http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/
 */
class BezierCurve : public Geometry
{
public:
    typedef std::shared_ptr<BezierCurve> Ptr;

    BezierCurve(glm::vec4* points, int32 degree, int32 segments = 128)
    : Geometry()
    , _control_points(nullptr)
    , _points_count(degree + 1)
    {}

    virtual ~BezierCurve()
    {
        SAFE_DEL_ARR(_control_points);
    }

    virtual void SetControlPoints(glm::vec4* points, int32 count) {}
    virtual void TranslatePoint(int32 index, glm::vec3 v) {}
    virtual void GenerateCurve()
    {

    }

private:
    void DeCasteljau();

private:
    glm::vec4* _control_points;
    int32 _points_count; //points count: n + 1;
};


// template<>
// class BezierCurve<2> : public Object
// {

// };

// template<>
// class BezierCurve<3> : public Object
// {

// };

// typedef BezierCurve<2> QuadraticBezierCurve;
// typedef BezierCurve<3> CubicBezierCurve;


struct Knot
{
    float32 u;
    int8 multiplity = 1;
};


/**
 * http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/
 */
class BSplineCurve : public Geometry
{
public:
    typedef std::shared_ptr<BSplineCurve> Ptr;

    BSplineCurve(int32 n, int32 p)
        : Geometry()
        , _control_points(nullptr)
        , _knots(nullptr)
        , _points_count(n)
        , _knots_count(n + p + 1)
        , _degree(p)
    {
        _control_points = new glm::vec4[_points_count];
        _knots = new Knot[_knots_count];
    }

    virtual ~BSplineCurve() 
    {
        SAFE_DEL_ARR(_control_points);
        SAFE_DEL_ARR(_knots);

        _mesh = nullptr;
    }

    int32 GetControlPointCount() { return _points_count; }
    int32 GetKnotsCount() { return _knots_count; }
    int32 GetDegree() { return _degree; }

    virtual void SetControlPoints(glm::vec3* points, int32 count)
    {
        if (_control_points != nullptr && points != nullptr)
        {
            int32 n = _points_count > count ? count : _points_count;
            for(int32 i =0; i < n; i++)
            {
                _control_points[i] = glm::vec4(points[i], 1.0f);
            }
        }
    }
    virtual void SetControlPoints(glm::vec4 *points, int32 count)
    {
        if (_control_points != nullptr && points != nullptr)
        {
            int32 n = _points_count > count ? count : _points_count;
            memcpy(_control_points, points, sizeof(glm::vec4) * n);
        }
    }
    virtual void SetKnots(Knot *knots, int32 count)
    {
        if (_knots != nullptr && knots != nullptr)
        {
            int32 m = _knots_count > count ? count : _knots_count;
            memcpy(_knots, knots, sizeof(Knot) * m);
        }
    }
    virtual void GenerateKnots()
    {
    }
    virtual void Clamp()
    {}
    virtual void Close()
    {}
    virtual void TranslatePoint(int32 index, glm::vec3 v)
    {}
    virtual void InsertKnot(float32 u, int32 multiple = 1)
    {}

    virtual void GenerateCurve(int32 segments = 64)
    {
        if(_mesh == nullptr)
        {
            _mesh = Mesh::CreateLineMesh(nullptr, (_knots_count - 1 - 2 * _degree) * segments + 1);
        }

        SubMesh::Ptr submesh = _mesh->GetSubMesh(0);
        glm::vec3* vertices = submesh->GetVerticePos();
        int32 n = 0;

        for(int32 i = _degree, span_count = _knots_count - 1 - _degree; i < span_count; i++)
        {
            float32 u_step = (_knots[i+1].u - _knots[i].u) / segments;

            int32 gen_points_count = segments;
            if(i == span_count - 1)
            {
                gen_points_count = segments + 1;;
            }
            for(int32 j = 0; j < gen_points_count; j++)
            {
                float32 u = _knots[i].u + u_step * j;

                vertices[n++] = DeBoor(u, i);

                // std::cout << "------------------------" << std::endl;
                // std::cout << "x:" << vertices[n - 1].x << std::endl;
                // std::cout << "y:" << vertices[n - 1].y << std::endl;
                // std::cout << "z:" << vertices[n - 1].z << std::endl;
            }
        }
    }

protected:
    /**
     * http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/single-insertion.html
     * http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/multiple-time.html
     * http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/de-Boor.html
     */
    glm::vec3 DeBoor(float32 u, int32 k)
    {
        int32 s = 0;
        if(u == _knots[k].u)
        {
            s = _knots[k].multiplity;
        }

        int32 h = _degree - s; //u insert to [U_k, U_k+1) times;
        if (h == 0)
        {
            return _control_points[k - _degree];
        }

        glm::vec4 helper[h];

        for(int32 r = 1; r <= h; r++)
        {
            for(int32 j = k - _degree + r, n = k - s, g = 1; j <= n; j++)
            {
                float32 a = (u - _knots[j].u) / (_knots[j + _degree - r + 1].u - _knots[j].u);
                if(r == 1)
                {
                    helper[g - 1] = (1.0f - a) * _control_points[j - 1] + a * _control_points[j];
                }
                else
                {
                    helper[g - 1] = (1.0f - a) * helper[g - 1] + a * helper[g];
                }

                g++;
            }
        }

        glm::vec3 result = glm::vec3(helper[0].x, helper[0].y, helper[0].z) / helper[0].w;

        return result;
    }

protected:
    glm::vec4 *_control_points;
    Knot *_knots;

    int32 _points_count; //points count: n + 1;
    int32 _knots_count; //knots count: m + 1;
    int32 _degree; //degree of curve
};

/**
 * http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/
 */
class NURBSCurve : public BSplineCurve
{
public:
    typedef std::shared_ptr<BSplineCurve> Ptr;

    NURBSCurve(int32 n, int32 p)
    : BSplineCurve(n, p)
    {}

    virtual ~NURBSCurve()
    {}

    void ModifyWeight(int32 index, float32 new_weight)
    {}

private:

};

/**
 * http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/
 */
class BezierSurface : public Geometry
{
public:
    typedef std::shared_ptr<BezierSurface> Ptr;


    BezierSurface(int32 n1, int32 n2)
    : Geometry()
    , _control_points(nullptr)
    , _v_knots(nullptr)
    , _u_knots(nullptr)
    , _u_points_count(n1)
    , _v_points_count(n2)
    , _u_degree(n1 - 1)
    , _v_degree(n2 - 1)
    {}

    void  Generate()
    {
        if(_mesh == nullptr)
        {

        }
    }

private:
    glm::vec3 DeCasteljau(float32 u, glm::vec3* points, int32 n)
    {
        return vec3();
    }

private:
    glm::vec4* _control_points;
    Knot* _v_knots;
    Knot* _u_knots;

    int32 _u_points_count;
    int32 _v_points_count;
    int32 _u_degree;
    int32 _v_degree;
};

/**
 * http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/
 */
class NURBSSurface : public Geometry
{
public:
    typedef std::shared_ptr<NURBSSurface> Ptr;

    NURBSSurface(int32 n1, int32 p1, int32 n2, int32 p2, int32 subdivide = 16, bool rational = false, MeshDataFlag flag = MeshDataFlag(13))
        : Geometry()
        , _control_points(nullptr)
        , _v_knots(nullptr)
        , _u_knots(nullptr)
        , _apdu_surface(nullptr)
        , _apdv_surface(nullptr)
        , _u_points_count(n1), _v_points_count(n2)
        , _u_knots_count(n1 + p1 + 1), _v_knots_count(n2 + p2 + 1)
        , _u_degree(p1)
        , _v_degree(p2)
        , _subdivide(subdivide)
        , _rational(rational)
    {
        _flag = flag;

        _control_points = new glm::vec4[_u_points_count * _v_points_count];
        _u_knots = new Knot[_u_knots_count];
        _v_knots = new Knot[_v_knots_count];
    }

    virtual ~NURBSSurface()
    {
        SAFE_DEL_ARR(_control_points);
        SAFE_DEL_ARR(_v_knots);
        SAFE_DEL_ARR(_u_knots);

        _mesh = nullptr;
    }

    void Clamp()
    {}
    void Close()
    {}
    void TranslatePoint(int32 index, glm::vec3 direction)
    {}
    void InsertKnot(float32 u, int32 multiplie = 1)
    {}

    void SetControlPoints(glm::vec4* points, int32 n)
    {
        int32 count = _u_points_count * _v_points_count;
        if(n < count) count = n;

        memcpy(_control_points, points, sizeof(glm::vec4) * count);
    }

    void SetUKnots(Knot* knots, int32 n)
    {
        if (_u_knots != nullptr && knots != nullptr)
        {
            int32 m = _u_knots_count > n ? n : _u_knots_count;
            memcpy(_u_knots, knots, sizeof(Knot) * m);
        }
    }

    void SetVKnots(Knot* knots, int32 n)
    {
        if (_v_knots != nullptr && knots != nullptr)
        {
            int32 m = _v_knots_count > n ? n : _v_knots_count;
            memcpy(_v_knots, knots, sizeof(Knot) * m);
        }
    }

    virtual void GenerateMesh() override
    {
        int32 v_count = (_v_knots_count - 1 - 2 * _v_degree) * _subdivide + 1;
        int32 u_count = (_u_knots_count - 1 - 2 * _u_degree) * _subdivide + 1;

        if(_mesh == nullptr)
        {
            SubMesh::Ptr subMesh = std::make_shared<SubMesh>();
            subMesh->Initialize(v_count * u_count,
                (v_count - 1) * (u_count - 1) * 6,
                _flag
            );

            _mesh = std::make_shared<Mesh>();
            _mesh->AddSubMesh(subMesh);

            InitializeAPDUSurface();
            InitializeAPDVSurface();
        }

        SubMesh::Ptr subMesh = _mesh->GetSubMesh(0);
        glm::vec3* vertices = subMesh->GetVerticePos();
        glm::vec3* normals = subMesh->GetVerticeNormal();
        glm::vec4* colos = subMesh->GetVerticeColor();
        glm::vec4* uvs = subMesh->GetVerticeUV();
        glm::uint32* indices = subMesh->GetIndice();
        int32 gen_point_index = 0;
        int32 gen_indice_index = 0;

        int32 v_span = _v_knots_count - 1 - _v_degree;
        int32 u_span = _u_knots_count - 1 - _u_degree;

        float32 u_step = 0.0f;
        float32 v_step = 0.0f;            

        for (int32 i = _v_degree; i < v_span; i++)
        {
            v_step = (_v_knots[i + 1].u - _v_knots[i].u) / _subdivide;

            int32 v_points_count = _subdivide;
            if (i == v_span - 1)
            {
                v_points_count = _subdivide + 1;
            }

            for (int32 j = 0; j < v_points_count; j++)
            {
                float32 v = _v_knots[i].u + v_step * j;

                for (int32 k = _u_degree; k < u_span; k++)
                {
                    u_step = (_u_knots[k + 1].u - _u_knots[k].u) / _subdivide;

                    int32 u_points_count = _subdivide;
                    if (k == u_span - 1)
                    {
                        u_points_count = _subdivide + 1;
                    }

                    for (int32 l = 0; l < u_points_count; l++)
                    {
                        float32 u = _u_knots[k].u + u_step * l;
                        glm::vec4 suv = GetPoint(u, v, k, i);
                        vertices[gen_point_index] = glm::vec3(suv.x, suv.y, suv.z) / suv.w;
                        normals[gen_point_index++] = ComputeNormal(u, 
                            v, 
                            k, 
                            i, 
                            glm::vec3(suv.x, suv.y, suv.z) / suv.w
                        );

                        // vertices[gen_point_index - 1] += normals[gen_point_index - 1] * 1.0f;

                        // std::cout << "-----------normal-------------" << std::endl;
                        // std::cout << "x:" << normals[gen_point_index - 1].x << std::endl;
                        // std::cout << "y:" << normals[gen_point_index - 1].y << std::endl;
                        // std::cout << "z:" << normals[gen_point_index - 1].z << std::endl;
                    }
                }
            }
        }

        for(int32 row = 0; row < v_count; row++)
        {
            if (row != v_count - 1)
            {
                for (int32 k = 0, g = u_count - 1; k < g; k++)
                {
                    indices[gen_indice_index++] = row * u_count + k;
                    indices[gen_indice_index++] = row * u_count + k + 1;
                    indices[gen_indice_index++] = (row + 1) * u_count + k + 1;

                    indices[gen_indice_index++] = row * u_count + k;
                    indices[gen_indice_index++] = (row + 1) * u_count + k + 1;
                    indices[gen_indice_index++] = (row + 1) * u_count + k;
                }
            }
        }
    }

    glm::vec4 GetPoint(float32 u, float32 v)
    {
        int32 v_span = FindKnotSpanOfV(v);
        int32 u_span = FindKnotSpanOfU(u);

        return GetPoint(u, v, u_span, v_span);
    }

    glm::vec4 GetPoint(float32 u, float32 v, int32 uspan, int32 vspan)
    {
        glm::vec4 u_points[_u_points_count];
        for(int32 col = 0; col < _u_points_count; col++)
        {
            u_points[col] = DeBoor(v, vspan, _v_degree, _v_knots, &(_control_points[_v_points_count * col]));
        }

        return DeBoor(u, uspan, _u_degree, _u_knots, u_points);
    }

    glm::vec4* GetControlPoints() { return _control_points; }
    Knot* GetUKnots() { return _u_knots; }
    Knot* GetVKnots() { return _v_knots; }

    int32 FindKnotSpanOfU(float32 u)
    {
        return FindKnotSpan(u, _u_knots, _u_degree, _u_knots_count - _u_degree - 1);
    }

    int32 FindKnotSpanOfV(float32 v)
    {
        return FindKnotSpan(v, _v_knots, _v_degree, _v_knots_count - _v_degree - 1);
    }

    int32 FindKnotSpan(float32 u, Knot* knots, int32 start, int32 end)
    {
        //Todo: fixed it with binary search

        for(int32 i = start, n = end - 1; i < n; i++)
        {
            if(i <= end - 2)
            {
                if (u >= knots[i].u && u < knots[i + 1].u)
                {
                    return i;
                }
            }
            else
            {
                if (u >= knots[i].u && u <= knots[i + 1].u)
                {
                    return i;
                }
            }
        }

        return -1;
    }

private:
    void InitializeAPDUSurface()
    {
        if(_apdu_surface == nullptr)
        {
            _apdu_surface = std::make_shared<NURBSSurface>(_u_points_count - 1, _u_degree - 1, _v_points_count, _v_degree);
            glm::vec4* points = _apdu_surface->GetControlPoints();

            for(int32 col = 0, col_count = _u_points_count - 1; col < col_count; col++)
            {
                for(int32 row = 0; row < _v_points_count; row++)
                {
                    int32 index = col * _v_points_count + row;
                    int32 index_p1 = index + _v_points_count;
                    int32 index_p0 = index;

                    points[index] = ((_control_points[index_p1] - _control_points[index_p0]) * (float32)_u_degree)
                        / (_u_knots[col + 1 + _u_degree].u - _u_knots[col + 1].u);

                    if(!_rational)
                    {
                        points[index].w = 1.0f;
                    }
                    else
                    {
                        float32 w = points[index].w;
                        if (w == 0.0f)
                        {
                            points[index].w = 1.0f;
                            w = _control_points[index_p1].w;
                        }

                        points[index] *= w;
                    }
                }
            }

            Knot* uknots = _apdu_surface->GetUKnots();
            memcpy(uknots, _u_knots + 1, (_u_knots_count - 2) * sizeof(Knot));

            Knot* vknots = _apdu_surface->GetVKnots();
            memcpy(vknots, _v_knots, _v_knots_count * sizeof(Knot));

            if (_u_knots[0].u == _u_knots[_u_degree].u)
            {
                uknots[_u_degree - 1].multiplity--;
            }
            if(_u_knots[_u_knots_count - 1].multiplity > 1)
            {
                uknots[_u_knots_count - 3].multiplity = _u_knots[_u_knots_count - 1].multiplity;
            }

            // for(int32 i = 0; i < _u_knots_count - 2; i++)
            // {
            //     std::cout << uknots[i].u << std::endl;
            // }
        }
    }

    void InitializeAPDVSurface()
    {
        if(_apdv_surface == nullptr)
        {
            _apdv_surface = std::make_shared<NURBSSurface>(_u_points_count, _u_degree, _v_points_count - 1, _v_degree - 1);
            glm::vec4* points = _apdv_surface->GetControlPoints();

            for(int32 col = 0; col < _u_points_count; col++)
            {
                for(int32 row = 0, row_count = _v_points_count - 1; row < row_count; row++)
                {
                    int32 index = col * row_count + row;
                    int32 index_p0 = col * _v_points_count + row;
                    int32 index_p1 = index_p0 + 1;

                    points[index] = ((_control_points[index_p1] - _control_points[index_p0]) * (float32)_v_degree)
                        / (_v_knots[row + 1 + _v_degree].u - _v_knots[row + 1].u);

                    if (!_rational)
                    {
                        points[index].w = 1.0f;
                    }
                    else
                    {
                        float32 w = points[index].w;
                        if (w == 0.0f)
                        {
                            points[index].w = 1.0f;
                            w = _control_points[index_p1].w;
                        }

                        points[index] *= w;
                    }
                }
            }

            Knot* vknots = _apdv_surface->GetVKnots();
            memcpy(vknots, _v_knots + 1, (_v_knots_count - 2) * sizeof(Knot));

            Knot* uknots = _apdv_surface->GetUKnots();
            memcpy(uknots, _u_knots, _u_knots_count * sizeof(Knot));

            if(_v_knots[0].u == _v_knots[_v_degree].u)
            {
                vknots[_v_degree - 1].multiplity--;
            }
            if (_v_knots[_v_knots_count - 1].multiplity > 1)
            {
                vknots[_v_knots_count - 3].multiplity = _v_knots[_v_knots_count - 1].multiplity;
            }

            // for(int32 i = 0; i < _v_knots_count - 2; i++)
            // {
            //     std::cout << vknots[i].u << std::endl;
            // }
        }
    }

    glm::vec4 DeBoor(float32 u, int32 k, int32 p, Knot* knots, glm::vec4* points)
    {
        int32 s = 0;
        if(u == knots[k].u)
        {
            s = knots[k].multiplity;
        }

        int32 h = p - s; //u insert to [U_k, U_k+1) times;
        if(h == 0)
        {
            return points[k - p];
        }

        glm::vec4 helper[h];

        for(int32 r = 1; r <= h; r++)
        {
            for(int32 j = k - p + r, n = k - s, g = 1; j <= n; j++)
            {
                float32 a = (u - knots[j].u) / (knots[j + p - r + 1].u - knots[j].u);
                if(r == 1)
                {
                    helper[g - 1] = (1.0f - a) * points[j - 1] + a * points[j];
                }
                else
                {
                    helper[g - 1] = (1.0f - a) * helper[g - 1] + a * helper[g];
                }

                g++;
            }
        }

        return helper[0];
    }


        /**
     * @brief 
     * 
     * @param u 
     * @param v 
     * @param i 
     * @param j 
     * @return glm::vec3 
     */
    glm::vec3 ComputeNormal(float32 u, float32 v, glm::vec3 suv)
    {
        int32 uspan = FindKnotSpanOfU(u);
        int32 vspan = FindKnotSpanOfV(v);

        return ComputeNormal(u, v, uspan, vspan, suv);
    }

    glm::vec3 ComputeNormal(float32 u, float32 v, int32 uspan, int32 vspan, glm::vec3 suv)
    {
        float32 w = _rational ? _control_points[uspan * _v_points_count + vspan].w : 1.0f;

        glm::vec4 apdu_point = _apdu_surface->GetPoint(u, v, uspan - 1, vspan);
        glm::vec3 apdu = glm::vec3(apdu_point.x, apdu_point.y, apdu_point.z) / apdu_point.w;
        float32 wpdu = _rational ? apdu_point.w : 0.0f;

        glm::vec4 apdv_point = _apdv_surface->GetPoint(u, v, uspan, vspan - 1);
        glm::vec3 apdv = glm::vec3(apdv_point.x, apdv_point.y, apdv_point.z) / apdv_point.w;
        float32 wpdv = _rational ? apdv_point.w : 0.0f;

        glm::vec3 pdu = glm::normalize((apdu - wpdu * suv) / w);
        glm::vec3 pdv = glm::normalize((apdv - wpdv * suv) / w);

        glm::vec3 normal = glm::normalize(glm::cross(pdu, pdv));
        return normal;
    }

    glm::vec3 ComputeDUV(float32 u, float32 v)
    {
        glm::vec3 duv;

        return duv;
    }

    /**
     * @brief 
     * S_u(u,v) = (A_u(u,v) - w(u,v) * S(u,v)) / w(u,v);
     * 
     * @param u 
     * @param v 
     * @param i 
     * @param j
     * @return glm::vec3 
     */
    glm::vec4 ComputeAPDU(float32 u, float32 v)
    {
        InitializeAPDUSurface();

        return _apdu_surface->GetPoint(u, v);
    }

    /**
     * @brief 
     * S_v(u,v) = (A_v(u,v) - w(u,v) * S(u,v)) / w(u,v);
     * 
     * @param u 
     * @param v 
     * @param i 
     * @param j 
     * @return glm::vec3 
     */
    glm::vec4 ComputeAPDV(float32 u, float32 v)
    {
        InitializeAPDVSurface();

        return _apdv_surface->GetPoint(u, v);
    }

private:
    //控制点列优先排列
    glm::vec4 *_control_points;
    Knot* _v_knots;
    Knot* _u_knots;
    NURBSSurface::Ptr _apdu_surface;
    NURBSSurface::Ptr _apdv_surface;

    int32 _u_points_count;
    int32 _v_points_count;
    int32 _u_knots_count;
    int32 _v_knots_count;
    int32 _u_degree;
    int32 _v_degree;

    int32 _subdivide;

    bool _rational;
};


} // namespace TwinkleGraphics


#endif