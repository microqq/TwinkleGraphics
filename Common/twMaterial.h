
#ifndef TW_MATERIAL_H
#define TW_MATERIAL_H

#include <vector>

#include "twCommon.h"
#include "twShader.h"
#include "twRenderContext.h"

namespace TwinkleGraphics
{

class RenderPass;

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

    const RenderState& GetRenderState() { return _state; }

    void AddRenderPass(const RenderPass::Ptr pass) { _passes.push_back(pass); }

private:
    std::vector<RenderPass::Ptr> _passes;
    RenderState _state;
};

class RenderPass : public Object
{
public:
    typedef std::shared_ptr<RenderPass> Ptr;

    RenderPass();
    virtual ~RenderPass();

    void SetShader(ShaderProgram::Ptr shader) { _shader = shader; }

    void SetCullMode(CullMode cull) {}
    void SetPolygonMode(PolygonMode polygonmode) {}

    const ShaderProgram::Ptr& GetShader() { return _shader; }
    const RenderState& GetRenderState() { return _state; }

private:
    RenderState _state;
    ShaderProgram::Ptr _shader;
};

} // namespace TwinkleGraphics


#endif