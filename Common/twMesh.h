

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

    void Initialize(int32 num, MeshDataFlag flag);
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

/**
 * http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/
 */
class BezierCurve
{

public:
    BezierCurve(int32 degree)
    : _control_points(nullptr)
    , _points_count(degree + 1)
    {}

    ~BezierCurve()
    {
        SAFE_DEL_ARR(_control_points);        
    }

    void SetControlPoints(glm::vec3* points, int32 count);
    void TranslatePoint(int32 index, glm::vec3 v);
    void GenerateCurve()
    {}

private:
    void DeCasteljau();

private:
    glm::vec3* _control_points;
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
    , _points_count(n + 1)
    , _knots_count(n + p + 2)
    , _degree(p)
    {
        _control_points = new glm::vec3[_points_count];
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

    void TranslatePoint(int32 index, glm::vec3 v)
    {}
    void InsertKnot(float32 u, int32 multiple = 1)
    {}
    void SetControlPoints(glm::vec3 *points, int32 count)
    {
        if(_control_points != nullptr && points != nullptr)
        {            
            int32 n = _points_count > count ? count : _points_count;
            memcpy(_control_points, points, sizeof(glm::vec3) * n);
        }
    }
    void SetKnots(Knot* knots, int32 count)
    {
        if(_knots != nullptr && knots != nullptr)
        {
            int32 m = _knots_count > count ? count : _knots_count;
            memcpy(_knots, knots, sizeof(Knot) * m);
        }
    }
    void GenerateKnots()
    {
        
    }
    void GenerateCurve(int32 segments = 64)
    {
        if(_mesh == nullptr)
        {
            _mesh = Mesh::CreateLineMesh(nullptr, (_knots_count - 1 - 2 * _degree) * segments);
        }

        SubMesh::Ptr submesh = _mesh->GetSubMesh(0);
        glm::vec3* vertices = submesh->GetVerticePos();
        int32 n = 0;

        glm::vec3* points_helper = new glm::vec3[_degree];

        for(int32 i = _degree, span_count = _knots_count - 1 - _degree; i < span_count; i++)
        {
            float32 u_step = (_knots[i+1].u - _knots[i].u) / segments;
            for(int32 j = 0; j < segments; j++)
            {
                float32 u = _knots[i].u + u_step * j;

                vertices[n++] = DeBoor(u, i);
            }
        }

        int32 a = 10;
    }

    Mesh::Ptr GetMesh() { return _mesh; }

private:
    /**
     * http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/single-insertion.html
     * http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/multiple-time.html
     * http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/de-Boor.html
     */
    glm::vec3 DeBoor(float32 u, int32 span_index)
    {
        int32 s = 0;
        if(u == _knots[span_index].u)
        {
            s = _knots[span_index].multiplity;
        }

        int32 h = _degree - s; //u insert to [U_k, U_k+1) times;
        glm::vec3 helper[h];
        glm::vec3* last = &(_control_points[span_index - s]);
        
        // for(int32 i = 0; i < h; i++)
        // {
        //     for(int32 j = 0, count = h - i; j < count; j++)
        //     {
        //         float32 a = (u - _knots[span_index - s - j].u) /
        //                     (_knots[span_index - s - j + _degree].u - _knots[span_index - s - j].u);

        //         glm::vec3 p0, p1;
        //         if (i == 0)
        //         {
        //             p1 = *(last - sizeof(glm::vec3) * j);
        //             p0 = *(last - sizeof(glm::vec3) * (j + 1));

        //             helper[h - j - 1] = (1 - a) * p0 + a * p1;
        //         }
        //         else
        //         {
        //             p1 = helper[h - j];
        //             p0 = helper[h - j - 1];

        //             helper[h - j] = (1 - a) * p0 + a * p1;
        //         }
        //     }
        // }

        return helper[0];
    }

private:
    glm::vec3 *_control_points;
    Knot *_knots;
    Mesh::Ptr _mesh;

    int32 _points_count; //points count: n + 1;
    int32 _knots_count; //knots count: m + 1;
    int32 _degree; //degree of curve
};

/**
 * http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/
 */
class NURBSCurve
{

};

/**
 * http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/
 */
class BezierSurface
{

};

/**
 * http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/
 */
class NURBSSurface
{

};

} // namespace TwinkleGraphics


#endif