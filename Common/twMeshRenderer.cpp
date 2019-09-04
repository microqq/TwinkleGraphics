
#include <algorithm>

#include "twMeshRenderer.h"



namespace TwinkleGraphics
{
MeshRenderer::MeshRenderer()
    : Object()
    , _mesh(nullptr)
    , _material(nullptr)
    , _shared_material(nullptr)
{

}   

MeshRenderer::~MeshRenderer()
{
    _mesh = nullptr;
    _materials.clear();
}

void MeshRenderer::SetMesh(Mesh::Ptr mesh)
{
    if(_mesh != mesh)
    {
        _mesh = mesh;
    }
}

void MeshRenderer::AddMaterial(Material::Ptr material)
{
    if(material == nullptr)
        return;

    std::vector<Material::Ptr>::iterator it = std::find(_materials.begin()
        , _materials.end()
        , material
    );
    if (it == _materials.end())
    {
        _materials.push_back(material);
    }
}

void MeshRenderer::SetMaterial(int32 index, Material::Ptr material)
{
    if(material == nullptr || 
        index >= _materials.size())
    {
        return;
    }

    _materials[index] = material;
}

} // namespace TwinkleGraphics
