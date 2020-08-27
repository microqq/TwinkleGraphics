
#include <algorithm>

#include "twMeshRenderer.h"



namespace TwinkleGraphics
{
MeshRenderer::MeshRenderer()
    : Object()
    , _material(nullptr)
    , _sharedMaterial(nullptr)
    , _mesh(nullptr)
    , _vbo(nullptr)
{

}

MeshRenderer::~MeshRenderer()
{
    _mesh = nullptr;
    _materials.clear();
    _vaos.clear();
    _vbo = nullptr;
    _ibos.clear();
}

void MeshRenderer::SetMesh(Mesh::Ptr mesh, bool setupVAO)
{
    if(_mesh != mesh)
    {
        _mesh = mesh;
    }

    if(setupVAO)
    {
        SetupVAOs();
    }
}

void MeshRenderer::SetupVAOs()
{
    if(_mesh == nullptr)
    {
        return;
    }

    _vbo = std::make_shared<VertexBufferObject>();

    MeshDataFlag texcoordFlags[8] = {
        MeshDataFlag::HAS_UV,
        MeshDataFlag::HAS_UV1,
        MeshDataFlag::HAS_UV2,
        MeshDataFlag::HAS_UV3,
        MeshDataFlag::HAS_UV4,
        MeshDataFlag::HAS_UV5,
        MeshDataFlag::HAS_UV6,
        MeshDataFlag::HAS_UV7
    };

    glm::vec4 *verticeUV = _mesh->GetVerticeUV();
    glm::vec4 *verticeUV1 = _mesh->GetVerticeUV1();
    glm::vec4 *verticeUV2 = _mesh->GetVerticeUV2();
    glm::vec4 *verticeUV3 = _mesh->GetVerticeUV3();
    glm::vec4 *verticeUV4 = _mesh->GetVerticeUV4();
    glm::vec4 *verticeUV5 = _mesh->GetVerticeUV5();
    glm::vec4 *verticeUV6 = _mesh->GetVerticeUV6();
    glm::vec4 *verticeUV7 = _mesh->GetVerticeUV7();

    glm::vec4 *uvs[8] = {
        verticeUV,
        verticeUV1,
        verticeUV2,
        verticeUV3,
        verticeUV4,
        verticeUV5,
        verticeUV6,
        verticeUV7
    };

    int32 count = _mesh->GetSubMeshCount();
    for(int i = 0; i < count; i++)
    {
        SubMesh::Ptr submesh = _mesh->GetSubMesh(i);

        VertexArrayObject::Ptr vao = std::make_shared<VertexArrayObject>();
        IndexBufferObject::Ptr ibo = std::make_shared<IndexBufferObject>();
        _vaos.push_back(vao);
        _ibos.push_back(ibo);

        vao->Bind();
        {
            ibo->Bind();
            ibo->InitBufferData(submesh->GetIndiceNum() * 4, submesh->GetIndice());

            const MeshDataFlag& flag = _mesh->GetMeshDataFlag();
            int32 verticeNum = _mesh->GetVerticeNum();
            int32 offset = 0;
            _vbo->Bind();
            if(MeshDataFlag::DEFAULT == flag)
            {
                _vbo->InitBufferData(verticeNum * 12, _mesh->GetVerticePos());
                BindingVertexBuffer(0, 3, 0, offset, sizeof(vec3));
            }
            else
            {
                _vbo->InitBufferData(_mesh->GetDataSize(), nullptr);
                _vbo->UpdateBufferData(offset, verticeNum * 12, _mesh->GetVerticePos());
                BindingVertexBuffer(0, 3, 0, offset, sizeof(vec3));
                offset += verticeNum * 12;

                if(MeshDataFlag::HAS_COLOR & flag)
                {
                    _vbo->UpdateBufferData(offset, verticeNum * 16, _mesh->GetVerticeColor());
                    BindingVertexBuffer(1, 4, 1, offset, sizeof(vec4));
                    offset += verticeNum * 16;
                }

                if(MeshDataFlag::HAS_NORMAL & flag)
                {
                    _vbo->UpdateBufferData(offset, verticeNum * 12, _mesh->GetVerticeNormal());
                    BindingVertexBuffer(2, 3, 2, offset, sizeof(vec3));
                    offset += verticeNum * 12;
                }
                if(MeshDataFlag::HAS_BINORMAL & flag)
                {
                    _vbo->UpdateBufferData(offset, verticeNum * 12, _mesh->GetVerticeBiNormal());
                    BindingVertexBuffer(3, 3, 3, offset, sizeof(vec3));
                }

                for(int32 k = 0; k < 8; k++)
                {
                    if(texcoordFlags[k] & flag)
                    {
                        _vbo->UpdateBufferData(offset, verticeNum * 16, uvs[k]);
                        BindingVertexBuffer(4 + k, 4, 4 + k, offset, sizeof(vec4));
                        offset += verticeNum * 16;
                    }
                }
            }
        }
        vao->UnBind();
    }
}

void MeshRenderer::BindingVertexBuffer(int32 attribIndex, int32 attribSize, int32 bindingIndex, int32 offset, int32 stride)
{
    glEnableVertexAttribArray(attribIndex);
    glBindVertexBuffer(bindingIndex, _vbo->GetResource().id, offset, stride);
    glVertexAttribFormat(attribIndex, attribSize, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(attribIndex, bindingIndex);
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
