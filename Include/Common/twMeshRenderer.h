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

    void AddMaterial(Material::Ptr material);

    void SetMaterial(Material::Ptr material) 
    { 
        _sharedMaterial = material;
    }

    void SetSharedMaterial(Material::Ptr material) 
    { 
        _sharedMaterial = material; 
    }

    const Material::Ptr GetMaterial()
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

    const Material::Ptr GetSharedMaterial()
    {
        return _sharedMaterial;
    }

    const Material::Ptr GetMaterial(int32 index)
    { 
        if(index < 0 || index >= _sharedMaterials.size())
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

    int32 GetMaterialIndex(Material::Ptr mat)
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
    void SetMesh(Mesh::Ptr mesh, bool setupVAO = false);
    void SetupVAOs();
    const Mesh::Ptr GetMesh() { return _mesh; }

    VertexArrayObject::Ptr GetVertexArrayObject(int32 index) 
    {
        if(index >= 0 && index < _vaos.size())
        {
            return _vaos[index];
        }

        return nullptr;
    }

    void PushDrawCommands();

private:
    void BindingVertexBuffer(int32 attribIndex, int32 attribSize, int32 bindingIndex, int32 offset, int32 stride);

protected:
    std::vector<Material::Ptr> _materials;
    std::vector<Material::Ptr> _sharedMaterials;
    Material::Ptr _material;
    Material::Ptr _sharedMaterial;
    Mesh::Ptr _mesh;

    std::vector<VertexArrayObject::Ptr> _vaos;
    VertexBufferObject::Ptr _vbo;
    std::vector<IndexBufferObject::Ptr> _ibos;

    int32 _fistInstantiateIndex = -1;
};

} // namespace TwinkleGraphics


#endif