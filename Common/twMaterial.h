
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

    template<class T, uint32 N, class ...Args>
    void SetSimpleUniformValue(const char* name, Args&&...args)
    {
        SimpleUniform<T, N> *uniform = nullptr;

        std::map<std::string, Uniform *>::iterator it = _uniforms.find(name);
        if (it == _uniforms.end())
        {
            uniform = new SimpleUniform<T, N>(name);
            AddUniform(name, uniform);
        }
        else
        {
            uniform = dynamic_cast<SimpleUniform<T, N> *>(it->second);
        }

        uniform->Set(std::forward<Args>(args)...);
    }

    template <class T, uint32 N>
    void SetVecUniformValue(const char *name, vec<N, T, defaultp>& value)
    {
        VecUniform<T, N> *uniform = nullptr;

        std::map<std::string, Uniform *>::iterator it = _uniforms.find(name);
        if (it == _uniforms.end())
        {
            uniform = new VecUniform<T, N>(name);
            AddUniform(name, uniform);
        }
        else
        {
            uniform = dynamic_cast<VecUniform<T, N> *>(it->second);
        }

        uniform->Set(value);
    }

    template <class T, uint32 Row, uint32 Column>
    void SetMatrixUniformValue(const char *name, mat<Row, Column, T, defaultp>& value)
    {
        MatUniform<T, Row, Column> *uniform = nullptr;

        std::map<std::string, Uniform *>::iterator it = _uniforms.find(name);
        if (it == _uniforms.end())
        {
            uniform = new MatUniform<T, Row, Column>(name);
            AddUniform(name, uniform);
        }
        else
        {
            uniform = dynamic_cast<MatUniform<T, Row, Column> *>(it->second);
        }

        uniform->Set(value);
    }

    const RenderState &GetRenderState()
    {
        return _state;
    }
    const Uniform* GetUniform(const char* name);

private:
    std::vector<RenderPass::Ptr> _passes;
    std::map<std::string, Uniform*> _uniforms;

    RenderState _state;
};


} // namespace TwinkleGraphics


#endif