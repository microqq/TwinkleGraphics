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

    void SetMesh(Mesh::Ptr mesh);
    void AddMaterial(Material::Ptr material);
    void SetMaterial(int32 index, Material::Ptr material);

    const Mesh::Ptr GetMesh() { return _mesh; }
    const Material::Ptr GetMaterial(int32 index)
    { 
        if(index >= _materials.size())
            return nullptr; 
        return _materials[index];
    }

    void PushDrawCommands();

protected:
    std::vector<Material::Ptr> _materials;
    Mesh::Ptr _mesh;
};

} // namespace TwinkleGraphics


#endif