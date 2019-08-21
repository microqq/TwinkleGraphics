#include "twMaterial.h"

namespace TwinkleGraphics
{
/*------------------------------RenderPass--------------------------*/

RenderPass::RenderPass()
    : Object()
    , _slots()
    , _shader(nullptr)
{
}

RenderPass::~RenderPass()
{
    _slots.clear();
    _uniformbindings.clear();
}

void RenderPass::SetTexture(const char *name, Texture::Ptr tex)
{
    if(tex == nullptr)
        return;

    TextureSlot slot;
    slot.texname = name;
    slot.tex = tex;
    slot.slot = _slots.size();

    if(_shader != nullptr)
    {
        ShaderProgramUse use(_shader);
        slot.location = _shader->GetUniformLocation(name);
    }
    else
    {
        slot.location = -1;
    }
    

    _slots.push_back(slot);
}

void RenderPass::SetUniform(const char *name, Uniform *uniform)
{
    if(_shader != nullptr)
    {
        int32 count = _shader->GetActiveUniformsCount();
        for(int32 i = 0; i < count; i++)
        {
            const char* unif_name = _shader->GetActiveUniform(i);
            if(::strcmp(name, unif_name) == 0)
            {
                UniformBinding binding;
                binding.uniform = uniform;
                binding.location = _shader->GetUniformLocation(name);

                _uniformbindings.push_back(binding);

                return;
            }
        }
    }
}

/*------------------------------Material--------------------------*/

Material::Material()
    : Object()
{

}

Material::~Material()
{
    _passes.clear();

    for(auto u : _uniforms)
    {
        SAFE_DEL(u.second);
    }
    _uniforms.clear();
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
{
    std::map<std::string, Uniform *>::iterator it = _uniforms.find(name);
    if(it != _uniforms.end())
    {
        SAFE_DEL(it->second);
        _uniforms.erase(name);
    }
}

const Uniform *Material::GetUniform(const char *name)
{
    std::map<std::string, Uniform*>::iterator it = _uniforms.find(name);
    if(it != _uniforms.end())
    {
        return it->second;
    }
}

} // namespace TwinkleGraphics
