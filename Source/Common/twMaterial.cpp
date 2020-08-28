#include "twMaterial.h"

namespace TwinkleGraphics
{
/*------------------------------RenderPass--------------------------*/

RenderPass::Ptr RenderPass::CreateRenderPassInstance(ShaderOption options[], int32 num)
{
    ShaderManagerInst shaderMgr;
    ShaderProgram::Ptr program = shaderMgr->ReadShaders(options, num);

    RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
    return pass;
}


RenderPass::RenderPass(ShaderProgram::Ptr shader)
    : Object()
    , _slots()
    , _shader(shader)
    , _enable(true)
{
}

RenderPass::RenderPass(const RenderPass &copy)
    : Object()
    , _slots()
    , _state(copy._state)
    , _shader(copy._shader)
    , _enable(copy._enable)
{
    // for(auto src : copy._slots)
    // {
    //     TextureSlot dest;
    //     dest.tex = src.tex;
    //     dest.slot = src.slot;
    //     dest.location = src.location;

    //     _slots.push_back(dest);
    // }
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

        std::map<std::string, TextureSlot>::iterator it = _slots.find(name);
        if(it == _slots.end())
        {
            char uniformName[128];
            int32 count = _shader->GetActiveUniformsCount();
            for (int32 i = 0; i < count; i++)
            {
                _shader->GetActiveUniform(i, uniformName);
                if (::strcmp(name, uniformName) == 0)
                {
                    int32 location = _shader->GetUniformLocation(name);
                    TextureSlot slot;
                    // slot.texname = name;
                    slot.tex = tex;
                    slot.slot = _slots.size();

                    slot.location = location;
                    _slots.insert(std::map<std::string, TextureSlot>::value_type(name, slot));

                    return;
                }
            }
        }
        else
        {
            TextureSlot& slot = it->second;
            slot.tex = tex;
        }        
    }
}

void RenderPass::SetUniform(const char *name, Uniform *uniform)
{
    if(_shader != nullptr)
    {
        // ShaderProgramUse use(_shader);
        std::map<std::string, UniformLocation>::iterator it = _uniformlocations.find(name);
        if(it == _uniformlocations.end())
        {
            char uniformName[128];
            int32 count = _shader->GetActiveUniformsCount();
            for(int32 i = 0; i < count; i++)
            {
                _shader->GetActiveUniform(i, uniformName);
                if(::strcmp(name, uniformName) == 0)
                {
                    UniformLocation binding;
                    binding.uniform = uniform;
                    binding.location = _shader->GetUniformLocation(name);

                    _uniformlocations.insert(std::map<std::string, UniformLocation>::value_type(name, binding));

                    return;
                }
            }
        }
        else
        {
            UniformLocation& location = it->second;
            location.uniform = uniform;
        }
    }
}


#ifdef TEMPORARY_USE
/**
 * @brief 
 * 
 */
void RenderPass::Apply()
{
    //Apply render states


    //Apply shader
}
#endif


/*------------------------------Material--------------------------*/

Material::Ptr Material::CreateMaterailInstance(RenderPass::Ptr passes[], int32 num)
{
    Material::Ptr material = std::make_shared<Material>();
    for(int32 i = 0; i < num; i++)
    {
        if(passes[i] != nullptr)
        {
            material->AddRenderPass(passes[i]);
        }
    }

    return material;
}

Material::Material()
    : Object()
    , _passes()
    , _uniforms()
    , _textures()
{

}

Material::Material(const Material &copy)
    : Object()
    , _passes()
    , _uniforms()
    , _textures()
    , _state(copy._state)
{
    for(auto srcPass : copy._passes)
    {
        RenderPass::Ptr pass = std::make_shared<RenderPass>(*(srcPass.get()));
        _passes.push_back(pass);
    }

    for(auto srcUniform : copy._uniforms)
    {
        Uniform* uniform = srcUniform.second->Clone();
        _uniforms.insert(std::map<std::string, Uniform*>::value_type(srcUniform.first, uniform));

        for(auto pass : _passes)
        {
            pass->SetUniform(uniform->name.c_str(), uniform);
        }
    }

    for(auto srcTex : copy._textures)
    {
        _textures.insert(std::map<std::string, Texture::Ptr>::value_type(srcTex.first, srcTex.second));

        for (auto pass : _passes)
        {
            pass->SetTexture(srcTex.first.c_str(), srcTex.second);
        }
    }
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
    SetTexture("mainTex", maintex);
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
    else
    {
        if(it->second != tex)
        {
            for (auto pass : _passes)
            {
                if (pass != nullptr)
                {
                    pass->SetTexture(name, tex);
                }
            }
        }
    }
}

Texture::Ptr Material::GetTexture(const char *name)
{
    std::map<std::string, Texture::Ptr>::iterator it = _textures.find(name);
    if (it == _textures.end())
    {
        return nullptr;
    }

    return it->second;
}

void Material::SetTextureTiling(const char *name, vec2& tiling)
{
    if (_passes.size() == 0)
    {
#ifdef _DEBUG

#endif
        return;
    }    

    std::string sname = name;
    sname += "Tiling";
    this->SetVecUniformValue<float32, 2>(sname.c_str(), tiling);
}

void Material::SetTextureOffset(const char *name, vec2& offset)
{
    if (_passes.size() == 0)
    {
#ifdef _DEBUG

#endif
        return;
    }    

    std::string sname = name;
    sname += "Offset";
    this->SetVecUniformValue<float32, 2>(sname.c_str(), offset);
}

} // namespace TwinkleGraphics
