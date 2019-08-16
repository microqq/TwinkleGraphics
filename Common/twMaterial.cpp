#include "twMaterial.h"

namespace TwinkleGraphics
{
/*------------------------------RenderPass--------------------------*/

RenderPass::RenderPass()
    : Object()
    , _shader(nullptr)
{
}

RenderPass::~RenderPass()
{
}

/*------------------------------Material--------------------------*/

Material::Material()
    : Object()
{

}

Material::~Material()
{
    
}

void Material::AddUniform(const char *name, Uniform *uniform)
{
    std::map<std::string, Uniform *>::iterator it = _uniforms.find(name);
    if(it == _uniforms.end())
    {
        _uniforms.insert(
            std::map<std::string, Uniform *>::value_type(name, uniform)
        );
    }
    else
    {
        SAFE_DEL(it->second);
        it->second = uniform;
    }
}

void Material::RemoveUniform(const char *name)
{}

} // namespace TwinkleGraphics
