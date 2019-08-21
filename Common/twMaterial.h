
#ifndef TW_MATERIAL_H
#define TW_MATERIAL_H

#include <vector>
#include <map>

#include "twCommon.h"
#include "twShader.h"
#include "twRenderContext.h"
#include "twTexture.h"

namespace TwinkleGraphics
{

class RenderPass : public Object
{
public:
    typedef std::shared_ptr<RenderPass> Ptr;

    RenderPass();
    virtual ~RenderPass();

    void SetCullMode(CullMode cull) {}
    void SetPolygonMode(PolygonMode polygonmode) {}

    void SetShader(ShaderProgram::Ptr shader) { _shader = shader; }
    void SetMaintexture(Texture::Ptr maintex) { SetTexture("main_tex", maintex); }
    void SetTexture(const char* name, Texture::Ptr tex);

    void SetUniform(const char* name, Uniform* uniform);

    const RenderState& GetRenderState() { return _state; }
    const ShaderProgram::Ptr& GetShader() { return _shader; }
    const std::vector<TextureSlot>& GetTextureSlots() { return _slots; }

private:
    std::vector<TextureSlot> _slots;
    std::vector<UniformBinding> _uniformbindings;
    RenderState _state;
    ShaderProgram::Ptr _shader;
};


/**
 * @brief 
 * 
 */
class Material : public Object
{
public:
    typedef std::shared_ptr<Material> Ptr;

    Material();
    virtual ~Material();

    void SetCullMode(CullMode cull) {}
    void SetPolygonMode(PolygonMode polygonmode) {}

    void AddRenderPass(const RenderPass::Ptr pass) { _passes.push_back(pass); }
    void AddUniform(const char* name, Uniform* uniform);
    void RemoveUniform(const char* name);

    template<class T, uint32 N>
    void SetUniformValue(const char* name, T&& args...)
    {
        std::map<std::string, Uniform *>::iterator it = _uniforms.find(name);
        if (it != _uniforms.end())
        {
            bool ismatrix = it->second->IsMatrix();
            bool isvector = it->second->IsVector();

            if(isvector)
            {
                VecUniform<T, N>* vecuniform = dynamic_cast<VecUniform<T, N>*>(it->second);
                vecuniform->Set(args);
            }
        }
    }

    const RenderState& GetRenderState() { return _state; }
    const Uniform* GetUniform(const char* name);

private:
    std::vector<RenderPass::Ptr> _passes;
    std::map<std::string, Uniform*> _uniforms;

    RenderState _state;
};


} // namespace TwinkleGraphics


#endif