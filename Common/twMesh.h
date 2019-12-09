

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


class SubMesh;
class Mesh;
class Line;
class Quad;
class Plane;
class Cube;
class UVSphere;
class NormalizedCubeSphere;
class IcosahedronSphere;
class BezierCurve;
// template<>
// class BezierCurve<2>;
// template<>
// class BezierCurve<3>;
class BSplineCurve;
class NURBSCurve;
class BezierSurface;
class NURBSSurface;

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

    void Initialize(int32 num, int32 indice_num = 0, MeshDataFlag flag = MeshDataFlag(13));
    glm::vec3* GetVerticePos() { return _vertice_pos; }
    int32 GetVerticeNum() { return _vertice_num; }
    glm::vec4* GetVerticeColor() { return _vertice_color; }
    glm::vec4* GetVerticeUV() { return _vertice_uv; }
    glm::vec3* GetVerticeBiNormal() { return _vertice_binormal; }
    glm::vec3* GetVerticeNormal() { return _vertice_normal; }
    uint32* GetIndice() { return _indice; }
    int32 GetIndiceNum() { return _indice_num; }

    int32 GetDataSize();

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
    friend class Line;
    friend class Quad;
    friend class Plane;
    friend class Cube;
    friend class UVSphere;
    friend class NormalizedCubeSphere;
    friend class IcosahedronSphere;
    friend class BezierCurve;
    friend class BSplineCurve;
    friend class NURBSCurve;
    friend class BezierSurface;
    friend class NURBSSurface;
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
#ifdef _DEBUG
        std::cout << "Mesh: Mesh Deconstruct.\n";
#endif
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
    inline uint32 GetSubMeshCount() { return _submeshes.size(); }

    SubMesh::Ptr GetSubMesh(int32 index)
    {
        if(index < _submeshes.size()) 
            return _submeshes[index]; 
        return nullptr;
    }
    
private:
    std::vector<SubMesh::Ptr> _submeshes;
};
} // namespace TwinkleGraphics


#endif