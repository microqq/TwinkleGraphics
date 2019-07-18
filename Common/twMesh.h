

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
    int8 multiplity;
};

/**
 * http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/
 */
class BSplineCurve
{
    BSplineCurve(int32 n, int32 m, int32 p)
    : _control_points(nullptr)
    , _knots(nullptr)
    , _mesh(nullptr)
    , _points_count(n + 1)
    , _knots_count(m + 1)
    , _degree(p)
    {
        _control_points = new glm::vec3[_points_count];
        _knots = new Knot[_knots_count];
    }

    ~BSplineCurve() 
    {
        SAFE_DEL_ARR(_control_points);
        SAFE_DEL_ARR(_knots);

        _mesh = nullptr;
    }

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
            int m = _knots_count > count ? count : _knots_count;
            memcpy(_knots, knots, sizeof(Knot) * m);
        }
    }
    void GenerateKnots()
    {
        
    }
    void GenerateCurve(int32 segments)
    {
        _mesh = std::make_shared<Mesh>();

        SubMesh::Ptr submesh = std::make_shared<SubMesh>();
        submesh->Initialize(_points_count, 
            (MeshDataFlag)((int32)(MeshDataFlag::DEFAULT) | (int32)(MeshDataFlag::HAS_NORMAL))
        );

        _mesh->AddSubMesh(submesh);
    }

    Mesh::Ptr GetMesh() { return _mesh; }

private:
    /**
     * http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/single-insertion.html
     * http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/multiple-time.html
     * http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/de-Boor.html
     */
    void DeBoor()
    {

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