

#ifndef TW_MESH_H
#define TW_MESH_H

#include <vector>
#include <iostream>

#include "twCommon.h"

namespace TwinkleGraphics
{
enum MeshDataFlag
{
    DEFAULT = 0,
    HAS_COLOR = 1,
    HAS_BINORMAL = 2,
    HAS_NORMAL = 4,
    HAS_UV = 8
};

enum class LineJointFlag
{
    MITER,
    BEVEL,
    ROUND
};

enum class LineEndCapFlag
{
    BUTT,
    ROUND,
    SQUARE
};


class Mesh;
class SubMesh : public Object
{
public:
    typedef std::shared_ptr<SubMesh> Ptr;
    typedef std::weak_ptr<SubMesh> WeakPtr;

    SubMesh()
        : _vertice_pos(nullptr)
        , _vertice_color(nullptr) 
        , _vertice_binormal(nullptr)
        , _vertice_normal(nullptr)
        , _vertice_uv(nullptr)
        , _indice(nullptr)
        , _data_flag(MeshDataFlag::DEFAULT)
        , _vertice_num(0)
        , _indice_num(0)
        , _index(-1)
    {}
    SubMesh(const SubMesh& copy) {}
    virtual ~SubMesh()
    {
        SAFE_DEL(_vertice_pos);
        SAFE_DEL(_vertice_color);
        SAFE_DEL(_vertice_binormal);
        SAFE_DEL(_vertice_normal);
        SAFE_DEL(_vertice_uv);
        SAFE_DEL(_indice);
    }

    void Initialize(int32 num, int32 indice_num = 0, MeshDataFlag flag = MeshDataFlag::DEFAULT);
    glm::vec3* GetVerticePos() { return _vertice_pos; }
    int32 GetVerticeNum() { return _vertice_num; }
    glm::vec4* GetVerticeColor() { return _vertice_color; }
    glm::vec4* GetVerticeUV() { return _vertice_uv; }
    glm::vec3* GetVerticeBiNormal() { return _vertice_binormal; }
    glm::vec3* GetVerticeNormal() { return _vertice_normal; }
    uint32* GetIndice() { return _indice; }
    int32 GetIndiceNum() { return _indice_num; }

private:
    glm::vec3* _vertice_pos;
    glm::vec4* _vertice_color;
    glm::vec3* _vertice_binormal;
    glm::vec3* _vertice_normal;
    glm::vec4* _vertice_uv;
    uint32* _indice;

    MeshDataFlag _data_flag;
    int32 _vertice_num;
    int32 _indice_num;
    int32 _index;

    friend class Mesh;
};

class Mesh : public Object
{
public:
    typedef std::shared_ptr<Mesh> Ptr;
    typedef std::weak_ptr<Mesh> WeakPtr;

    static Ptr CreateLineMesh(glm::vec3* points, int32 num);
    static Ptr CreateLineMeshEx(glm::vec4* points, int32 num);
    static Ptr CreateQuadMesh(float32 x, float32 y);
    static Ptr CreateCubeMesh(float32 size);
    static Ptr CreateSphereMeshStandard(float32 radius, int32 longitude_count, int32 latitude_count);
    static Ptr CreateSphereMeshNormalizedCube(float32 radius, int32 subdivide);
    static Ptr CreateSphereMeshIcosahedron(float32 radius, int32 subdivide);
    static Ptr CreateBezierLine(glm::vec3* points, int32 num, int32 segments = 128);
    static Ptr CreateQuadraticBezierLine(glm::vec3* points, int32 segments = 128);
    static Ptr CreateCubicBezierLine(glm::vec3* points, int32 segments = 128);

private:
    static void CreateIconsahedron(glm::vec3 *vertice, uint32* indice, float32 radius);
    static glm::vec3 DeCasteljau(glm::vec3* points, glm::vec3* helper, int32 num, float32 u);

public:
    Mesh() {}
    virtual ~Mesh() 
    { 
        std::cout << "Mesh Deconstruct.\n";
        _submeshes.clear();
    }

    void AddSubMesh(SubMesh::Ptr submesh) 
    { 
        submesh->_index = _submeshes.size();
        _submeshes.push_back(submesh);  
    }
    void RemoveSubMesh(int32 index)
    {
        if(index < _submeshes.size())
            _submeshes.erase(_submeshes.begin() + index);
    }
    inline uint32 GetSubMeshNum() { return _submeshes.size(); }

    SubMesh::Ptr GetSubMesh(int32 index)
    {
        if(index < _submeshes.size()) 
            return _submeshes[index]; 
        return nullptr;
    }
    
private:
    std::vector<SubMesh::Ptr> _submeshes;
};

class UVShpere : public Object
{

};

class NormalizedCubeSphere : public Object
{

};

class IcosahedronSphere : public Object
{

};

class Cube : public Object
{

};

class Quad : public Object
{

};

class Line : public Object
{
public:
    typedef std::shared_ptr<Line> Ptr;

    Line()
    {}
    virtual ~Line()
    {}

protected:
};

/**
 * http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/
 */
class BezierCurve : public Object
{
public:
    typedef std::shared_ptr<BezierCurve> Ptr;

    BezierCurve(int32 degree)
    : _control_points(nullptr)
    , _points_count(degree + 1)
    {}

    ~BezierCurve()
    {
        SAFE_DEL_ARR(_control_points);
    }

    virtual void SetControlPoints(glm::vec4* points, int32 count);
    virtual void TranslatePoint(int32 index, glm::vec3 v);
    virtual void GenerateCurve()
    {

    }

private:
    void DeCasteljau();

private:
    glm::vec4* _control_points;
    int32 _points_count; //points count: n + 1;
};

struct Knot
{
    float32 u;
    int8 multiplity = 1;
};

/**
 * http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/
 */
class BSplineCurve : public Object
{
public:
    typedef std::shared_ptr<BSplineCurve> Ptr;

    BSplineCurve(int32 n, int32 p)
        : _control_points(nullptr)
        , _knots(nullptr)
        , _mesh(nullptr)
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

    Mesh::Ptr GetMesh() { return _mesh; }

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
    Mesh::Ptr _mesh;

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
class BezierSurface : public Object
{
public:
    BezierSurface(int32 n1, int32 n2)
    : _control_points(nullptr)
    , _v_knots(nullptr)
    , _u_knots(nullptr)
    , _mesh(nullptr)
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
    {}

private:
    glm::vec4* _control_points;
    Knot* _v_knots;
    Knot* _u_knots;
    Mesh::Ptr _mesh;

    int32 _u_points_count;
    int32 _v_points_count;
    int32 _u_degree;
    int32 _v_degree;
};

/**
 * http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/
 */
class NURBSSurface : public Object
{
public:
    typedef std::shared_ptr<NURBSSurface> Ptr;

    NURBSSurface(int32 n1, int32 p1, int32 n2, int32 p2, int32 subdivide = 16)
        : _control_points(nullptr)
        , _v_knots(nullptr)
        , _u_knots(nullptr)
        , _mesh(nullptr)
        , _apdu_surface(nullptr)
        , _apdv_surface(nullptr)
        , _u_points_count(n1), _v_points_count(n2)
        , _u_knots_count(n1 + p1 + 1), _v_knots_count(n2 + p2 + 1)
        , _u_degree(p1), _v_degree(p2)
        , _subdivide(subdivide)
    {
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

    void GenerateSurface()
    {
        int32 v_count = (_v_knots_count - 1 - 2 * _v_degree) * _subdivide + 1;
        int32 u_count = (_u_knots_count - 1 - 2 * _u_degree) * _subdivide + 1;

        if(_mesh == nullptr)
        {
            SubMesh::Ptr subMesh = std::make_shared<SubMesh>();
            subMesh->Initialize(v_count * u_count,
                (v_count - 1) * (u_count - 1) * 6,
                (MeshDataFlag)((int32)HAS_NORMAL | (int32)HAS_COLOR | (int32)HAS_UV)
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
                        normals[gen_point_index++] = ComputeNormal(u, v, k, i, suv);

                        // vertices[gen_point_index - 1] += normals[gen_point_index - 1] * 1.0f;

                        // std::cout << "-----------normal-------------" << std::endl;
                        // std::cout << "x:" << normals[gen_point_index - 1].x << std::endl;
                        // std::cout << "y:" << normals[gen_point_index - 1].y << std::endl;
                        // std::cout << "z:" << normals[gen_point_index - 1].z << std::endl;
                    }
                }
            }
        }

        // glm::vec4 *v_points = new glm::vec4[v_count * _u_points_count];
        // int32 v_index = 0;
        // //http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/surface/bspline-de-boor.html
        // //稍微改了下计算顺序，这里按列优先计算（控制点按列优先顺序排列）
        // for(int32 col = 0; col < _u_points_count; col++)
        // {
        //     for (int32 i = _v_degree; i < v_span; i++)
        //     {
        //         v_step = (_v_knots[i + 1].u - _v_knots[i].u) / _subdivide;

        //         int32 gen_points_count = _subdivide;
        //         if(i == v_span - 1)
        //         {
        //             gen_points_count = _subdivide + 1;
        //         }

        //         for (int32 j = 0; j < gen_points_count; j++)
        //         {
        //             float32 v = _v_knots[i].u + v_step * j;

        //             v_points[v_index++] = DeBoor(v, 
        //                 i, 
        //                 _v_degree, 
        //                 _v_knots, 
        //                 &(_control_points[_v_points_count * col])
        //             );

        //             // std::cout << "------------------------" << std::endl;
        //             // std::cout << "x:" << v_points[v_index - 1].x << std::endl;
        //             // std::cout << "y:" << v_points[v_index - 1].y << std::endl;
        //             // std::cout << "z:" << v_points[v_index - 1].z << std::endl;
        //         }
        //     }
        // }

        // glm::vec4* u_points = new glm::vec4[_u_points_count];
        // //接下来按行序——也就是 u 方向来计算表面f(u,v)上的点
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
            // //copy control points
            // for(int32 i = 0; i < _u_points_count; i++)
            // {
            //     u_points[i] = v_points[row + i * v_count];
            // }

            // for (int32 i = _u_degree; i < u_span; i++)
            // {
            //     u_step = (_u_knots[i + 1].u - _u_knots[i].u) / _subdivide;

            //     int32 gen_points_count = _subdivide;
            //     if (i == u_span - 1)
            //     {
            //         gen_points_count = _subdivide + 1;
            //     }

            //     for (int32 j = 0; j < gen_points_count; j++)
            //     {
            //         float32 u = _u_knots[i].u + u_step * j;

            //         glm::vec4 result = DeBoor(u, i, _u_degree, _u_knots, u_points); 
            //         vertices[gen_point_index++] = glm::vec3(result.x, result.y, result.z) / result.w;

            //         // std::cout << "------------------------" << std::endl;
            //         // std::cout << "x:" << vertices[gen_point_index - 1].x << std::endl;
            //         // std::cout << "y:" << vertices[gen_point_index - 1].y << std::endl;
            //         // std::cout << "z:" << vertices[gen_point_index - 1].z << std::endl;
            //     }
            // }
        }

        // SAFE_DEL_ARR(v_points);
        // SAFE_DEL_ARR(u_points);
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


    Mesh::Ptr GetMesh() { return _mesh; }

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

                    float32 w = points[index].w;
                    if(w == 0.0f)
                    {
                        points[index].w = 1.0f;
                        w = _control_points[index_p1].w;
                    }

                    points[index] *= w;

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
                uknots[_u_knots_count - 3].multiplity = _u_knots[_u_knots_count - 1].multiplity - 1;
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
                    int32 index_p1 = index + 1;
                    int32 index_p0 = index;

                    points[index] = ((_control_points[index_p1] - _control_points[index_p0]) * (float32)_v_degree)
                        / (_v_knots[row + 1 + _v_degree].u - _v_knots[row + 1].u);

                    float32 w = points[index].w;
                    if(w == 0.0f)
                    {
                        points[index].w = 1.0f;
                        w = _control_points[index_p1].w;
                    }

                    points[index] *= w;
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
                vknots[_v_knots_count - 3].multiplity = _v_knots[_v_knots_count - 1].multiplity - 1;
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
        float32 w = _control_points[uspan * _v_points_count + vspan].w;

        glm::vec4 apdu_point = _apdu_surface->GetPoint(u, v, uspan - 1, vspan);
        glm::vec3 apdu = glm::vec3(apdu_point.x, apdu_point.y, apdu_point.z) / apdu_point.w;
        float32 wpdu = 0.0f; //apdu_point.w;

        glm::vec4 apdv_point = _apdv_surface->GetPoint(u, v, uspan, vspan - 1);
        glm::vec3 apdv = glm::vec3(apdv_point.x, apdv_point.y, apdv_point.z) / apdv_point.w;
        float32 wpdv = 0.0f; //apdv_point.w;

        glm::vec3 pdu = (apdu - wpdu * suv) / w;
        glm::vec3 pdv = (apdv - wpdv * suv) / w;

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
    Mesh::Ptr _mesh;
    NURBSSurface::Ptr _apdu_surface;
    NURBSSurface::Ptr _apdv_surface;

    int32 _u_points_count;
    int32 _v_points_count;
    int32 _u_knots_count;
    int32 _v_knots_count;
    int32 _u_degree;
    int32 _v_degree;

    int32 _subdivide;
};

} // namespace TwinkleGraphics


#endif