#ifndef TW_MESHRENDER_H
#define TW_MESHRENDER_H

#include <vector>

#include "twCommon.h"
#include "twMesh.h"
#include "twMaterial.h"

namespace TwinkleGraphics
{
class MeshRenderer : public Object
{
public:
    typedef std::shared_ptr<MeshRenderer> Ptr;

    MeshRenderer();
    virtual ~MeshRenderer();


    void PushDrawCommands();

    void AddMaterial(Material::Ptr material);
    void SetMaterial(int32 index, Material::Ptr material);

    void SetMaterial(Material::Ptr material) { _shared_material = material; }
    void SetSharedMaterial(Material::Ptr material) { _shared_material = material; }
    void SetMesh(Mesh::Ptr mesh);

    const Material::Ptr GetMaterial()
    {
        if(_shared_material != _material)
        {
            _material = std::make_shared<Material>(*(_shared_material.get()));
            _shared_material = _material;
        }

        return _material;
    }
    const Material::Ptr GetSharedMaterial()
    {
        return _shared_material;
    }

    const Material::Ptr GetMaterial(int32 index)
    { 
        if(index >= _materials.size())
            return nullptr; 
        return _materials[index];
    }

    const Mesh::Ptr GetMesh() { return _mesh; }

protected:
    std::vector<Material::Ptr> _materials;
    Material::Ptr _material;
    Material::Ptr _shared_material;
    Mesh::Ptr _mesh;

};

} // namespace TwinkleGraphics


#endif