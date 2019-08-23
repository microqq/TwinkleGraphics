#include "twMaterial.h"

namespace TwinkleGraphics
{
/*------------------------------RenderPass--------------------------*/

RenderPass::RenderPass(ShaderProgram::Ptr shader)
    : Object()
    , _slots()
    , _shader(shader)
    , _enable(true)
{
}

RenderPass::~RenderPass()
{
    _slots.clear();
    _uniformlocations.clear();
}

void RenderPass::SetTexture(const char *name, Texture::Ptr tex)
{
    if(tex == nullptr)
        return;

    if(_shader != nullptr)
    {
        // ShaderProgramUse use(_shader);

        char unif_name[128];
        int32 count = _shader->GetActiveUniformsCount();
        for(int32 i = 0; i < count; i++)
        {
            _shader->GetActiveUniform(i, unif_name);
            if(::strcmp(name, unif_name) == 0)
            {
                int32 location = _shader->GetUniformLocation(name);
                TextureSlot slot;
                // slot.texname = name;
                slot.tex = tex;
                slot.slot = _slots.size();

                slot.location = location;
                _slots.push_back(slot);

                return;
            }
        }
    }
}

void RenderPass::SetUniform(const char *name, Uniform *uniform)
{
    if(_shader != nullptr)
    {
        // ShaderProgramUse use(_shader);

        char unif_name[128];
        int32 count = _shader->GetActiveUniformsCount();
        for(int32 i = 0; i < count; i++)
        {
            _shader->GetActiveUniform(i, unif_name);
            if(::strcmp(name, unif_name) == 0)
            {
                UniformLocation binding;
                binding.uniform = uniform;
                binding.location = _shader->GetUniformLocation(name);

                _uniformlocations.push_back(binding);

                return;
            }
        }
    }
}

/*------------------------------Material--------------------------*/

Material::Material()
    : Object()
    , _passes()
    , _uniforms()
    , _textures()
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

    _textures.clear();
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

    return nullptr;
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

void Material::SetPassesUniform(const char *name, Uniform *uniform)
{    
    if(_passes.size() == 0) return;

    for (auto pass : _passes)
    {
        if (pass != nullptr)
        {
            pass->SetUniform(name, uniform);
        }
    }
}

void Material::SetMainTexture(Texture::Ptr maintex)
{
    SetTexture("main_tex", maintex);
}

void Material::SetTexture(const char *name, Texture::Ptr tex)
{
    if (_passes.size() == 0)
        return;

    std::map<std::string, Texture::Ptr>::iterator it = _textures.find(name);
    if (it == _textures.end())
    {
        _textures.insert(std::map<std::string, Texture::Ptr>::value_type(name, tex));

        for (auto pass : _passes)
        {
            if (pass != nullptr)
            {
                pass->SetTexture(name, tex);
            }
        }
    }
}

} // namespace TwinkleGraphics