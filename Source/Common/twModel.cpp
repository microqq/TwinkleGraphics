#include "twModel.h"

namespace TwinkleGraphics
{
    Model::Model()
        : _rootGeometry(nullptr)
    {

    }

    Model::~Model()
    {
        _geometries.clear();
    }

    void Model::SetValid(bool valid)
    {
        if(_rootGeometry == nullptr)
        {
            return;
        }

        for(auto geom : _geometries)
        {
            MeshRenderer::Ptr renderer = geom->GetMeshRenderer();
            if(renderer == nullptr)
            {
                continue;
            }

            renderer->SetupVAOs();
        }

        _valid = valid;
    }

    void Model::AddGeometry(Geometry::Ptr geom)
    {
        if(geom != nullptr)
        {
            if(_rootGeometry == nullptr)
            {
                _rootGeometry = geom;
            }
            _geometries.push_back(geom);
        }
    }

    void Model::RemoveGeometry(int index)
    {
        int size = _geometries.size();
        if(index >= 0 && index < size)
        {
            _geometries.erase(_geometries.begin() + index);
        }
    }

    Geometry::Ptr Model::GetGeometry(int index)
    {
        int size = _geometries.size();
        if(index >= 0 && index < size)
        {
            return _geometries[index];
        }

        return nullptr;
    }

    /**
     * just for test
    */
    void Model::Render()
    {
        if(_rootGeometry == nullptr)
        {
            return;
        }

        for(auto geom : _geometries)
        {
            MeshRenderer::Ptr renderer = geom->GetMeshRenderer();
            if(renderer == nullptr)
            {
                continue;
            }

            int32 submeshCount = geom->GetMesh()->GetSubMeshCount();
            for(int i = 0; i < submeshCount; i++)
            {
                SubMesh::Ptr submesh = geom->GetMesh()->GetSubMesh(i);
                VertexArrayObject::Ptr vao = renderer->GetVertexArrayObject(i);
                if(vao == nullptr)
                {
                    continue;
                }

                Material::Ptr mat = renderer->GetMaterial(submesh->GetMaterialIndex());
                if(mat != nullptr)
                {
                    RenderPass::Ptr pass = mat->GetRenderPass(0);
                    if(pass == nullptr)
                    {
                        continue;
                    }

                    ShaderProgram::Ptr shader = pass->GetShaderProgram();

                    for (auto tex_slot : pass->GetTextureSlots())
                    {
                        tex_slot.second.Apply();
                    }

                    ShaderProgramUse use(shader);
                    for (auto loc : pass->GetUniformLocations())
                    {
                        loc.second.Bind();
                    }

                    glFrontFace(GL_CCW);
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

                    glDisable(GL_CULL_FACE);
                    glEnable(GL_DEPTH_TEST);
                    glDepthFunc(GL_LEQUAL);

                    //draw command use vertex array object
                    glBindVertexArray(vao->GetResource().id);
                    glDrawElements(GL_TRIANGLES, submesh->GetIndiceNum(), GL_UNSIGNED_INT, NULL);

                    for (auto tex_slot : pass->GetTextureSlots())
                    {
                        tex_slot.second.UnBind();
                    }
                }
            }
        }
    }
}