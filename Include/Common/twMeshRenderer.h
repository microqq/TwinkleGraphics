#ifndef TW_MESHRENDER_H
#define TW_MESHRENDER_H

#include <vector>

#include "twCommon.h"
#include "twMesh.h"
#include "twMaterial.h"
#include "twHWBuffers.h"

namespace TwinkleGraphics
{
class __TWCOMExport MeshRenderer : public Object
{
public:
    typedef std::shared_ptr<MeshRenderer> Ptr;

    MeshRenderer();
    virtual ~MeshRenderer();

    void AddMaterial(MaterialPtr material);

    void SetMaterial(MaterialPtr material) 
    { 
        _sharedMaterial = material;
    }

    void SetSharedMaterial(MaterialPtr material) 
    { 
        _sharedMaterial = material; 
    }

    const MaterialPtr GetMaterial()
    {
        if(_sharedMaterial == nullptr)
        {
            return nullptr;
        }

        if(_sharedMaterial != _material)
        {
            // _material = std::make_shared<Material>(*(_sharedMaterial.get()));
            _material = _sharedMaterial->SharedClone();
            if(!_material->IsValid())
            {
                _material->Initialize();
            }
            _sharedMaterial = _material;
        }

        return _material;
    }

    const MaterialPtr GetSharedMaterial()
    {
        return _sharedMaterial;
    }

    const MaterialPtr GetMaterial(int32 index)
    { 
        int size = _sharedMaterials.size();
        if(index < 0 || index >= size)
        {
            return nullptr; 
        }

        if(_sharedMaterials[index] == nullptr)
        {
            return nullptr;
        }

        if(_sharedMaterials[index] == _materials[index])
        {
            return _materials[index];
        }

        // _materials[index] = std::make_shared<Material>(*(_sharedMaterials[index].get()));
        _materials[index] = _sharedMaterials[index]->SharedClone();
        if (!_materials[index]->IsValid())
        {
            _materials[index]->Initialize();
        }

        _sharedMaterials[index] = _materials[index];

        return _materials[index];
    }

    int32 GetMaterialIndex(MaterialPtr mat)
    {
        if(mat == nullptr)
        {
            return -1;
        }

        int32 index = -1;
        for(auto m : _sharedMaterials)
        {
            ++index;
            if(mat == m)
            {
                return index;
            }
        }

        return index;
    }

    int32 GetMaterialCount() { return _materials.size(); }
    int32 GetSharedMaterialCount() { return _sharedMaterials.size(); }

    //Todo: Shared mesh
    void SetMesh(MeshPtr mesh, bool setupVAO = false);
    void SetupVAOs();
    const MeshPtr GetMesh() { return _mesh; }

    VertexArrayObjectPtr GetVertexArrayObject(int32 index) 
    {
        int32 size = _vaos.size();
        if(index >= 0 && index < size)
        {
            return _vaos[index];
        }

        return nullptr;
    }

    void PushDrawCommands();

private:
    void BindingVertexBuffer(int32 attribIndex, int32 attribSize, int32 bindingIndex, int32 offset, int32 stride);

protected:
    std::vector<MaterialPtr> _materials;
    std::vector<MaterialPtr> _sharedMaterials;
    MaterialPtr _material;
    MaterialPtr _sharedMaterial;
    MeshPtr _mesh;

    std::vector<VertexArrayObjectPtr> _vaos;
    VertexBufferObjectPtr _vbo;
    std::vector<IndexBufferObjectPtr> _ibos;

    int32 _fistInstantiateIndex = -1;
};

typedef MeshRenderer::Ptr MeshRendererPtr;

} // namespace TwinkleGraphics


#endif