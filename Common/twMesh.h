

#ifndef TW_MESH_H
#define TW_MESH_H

#include <vector>

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
        , _vertice_num(0)
        , _data_flag(MeshDataFlag::DEFAULT)
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
    glm::vec4* GetVerticeColor() { return _vertice_color; }
    glm::vec4* GetVerticeUV() { return _vertice_uv; }
    glm::vec3* GetVerticeBiNormal() { return _vertice_binormal; }
    glm::vec3* GetVerticeNormal() { return _vertice_normal; }

private:
    glm::vec3* _vertice_pos;
    glm::vec4* _vertice_color;
    glm::vec3* _vertice_binormal;
    glm::vec3* _vertice_normal;
    glm::vec4* _vertice_uv;
    uint32* _indice;

    MeshDataFlag _data_flag;
    int32 _vertice_num;
    int32 _indice_count;
    int32 _index;

    friend class Mesh;
};

class Mesh : public Object
{
public:
    typedef std::shared_ptr<Mesh> Ptr;
    typedef std::weak_ptr<Mesh> WeakPtr;

    static Ptr CreateQuadMesh(float32 x, float32 y);
    static Ptr CreateCubeMesh(float32 size);
    static Ptr CreateSphereMeshStandard(float32 radius, int32 longtitude_count, int32 lantitude_count);
    static Ptr CreateSphereMeshNormalizedCube(float32 radius, int32 subdivide);
    static Ptr CreateSphereMeshIcosahedron(float32 radius, int32 subdivide);

    Mesh() {}
    virtual ~Mesh() { _submeshes.clear(); }

    void AddSubMesh(SubMesh::Ptr submesh) 
    { 
        submesh->_index = _submeshes.size();
        _submeshes.push_back(submesh);  
    }
    void RemoveSubMesh(int index)
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

} // namespace TwinkleGraphics


#endif