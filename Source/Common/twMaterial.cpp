#include "twMaterial.h"

namespace TwinkleGraphics {
/*------------------------------RenderPass--------------------------*/

RenderPassPtr RenderPass::CreateRenderPassInstance(ShaderOption options[],
                                                   int32 num) {
  ShaderManager &shaderMgr = ShaderMgrInstance();
  ShaderProgramPtr program = shaderMgr.ReadShaders(options, num);

  RenderPassPtr pass = std::make_shared<RenderPass>(program);
  return pass;
}

RenderPass::RenderPass(ShaderProgramPtr shader)
    : Object(), _slots(), _program(shader), _enable(true) {}

RenderPass::RenderPass(const RenderPass &src)
    : Object(), _slots(), _state(src._state), _program(src._program),
      _enable(src._enable) {
  for (auto srcSlot : src._slots) {
    TextureSlot dest;
    dest.tex = srcSlot.second.tex;
    dest.slot = srcSlot.second.slot;
    dest.location = srcSlot.second.location;

    _slots.insert(std::make_pair(srcSlot.first, dest));
  }

  for (auto srcLocation : src._uniformlocations) {
    UniformLocation location;
    location.location = srcLocation.second.location;
    location.uniform = srcLocation.second.uniform;

    _uniformlocations.insert(std::make_pair(srcLocation.first, location));
  }
}

RenderPass::~RenderPass() {
  _slots.clear();
  _uniformlocations.clear();
}

void RenderPass::SetTexture(const char *name, TexturePtr tex) {
  if (tex == nullptr)
    return;

  if (_program != nullptr) {
    // ShaderProgramUse use(_shader);

    std::map<std::string, TextureSlot>::iterator it = _slots.find(name);
    if (it == _slots.end()) {
      char uniformName[128];
      int32 count = _program->GetActiveUniformsCount();
      for (int32 i = 0; i < count; i++) {
        _program->GetActiveUniform(i, uniformName);
        if (::strcmp(name, uniformName) == 0) {
          int32 location = _program->GetUniformLocation(name);
          TextureSlot slot;
          // slot.texname = name;
          slot.tex = tex;
          slot.slot = _slots.size();

          slot.location = location;
          _slots.insert(
              std::map<std::string, TextureSlot>::value_type(name, slot));

          return;
        }
      }
    } else {
      TextureSlot &slot = it->second;
      slot.tex = tex;
    }
  }
}

void RenderPass::SetUniform(const char *name, Uniform *uniform) {
  if (_program != nullptr) {
    // ShaderProgramUse use(_shader);
    std::map<std::string, UniformLocation>::iterator it =
        _uniformlocations.find(name);
    if (it == _uniformlocations.end()) {
      char uniformName[128];
      int32 count = _program->GetActiveUniformsCount();
      for (int32 i = 0; i < count; i++) {
        _program->GetActiveUniform(i, uniformName);
        if (::strcmp(name, uniformName) == 0) {
          UniformLocation binding;
          binding.uniform = uniform;
          binding.location = _program->GetUniformLocation(name);

          _uniformlocations.insert(
              std::map<std::string, UniformLocation>::value_type(name,
                                                                 binding));

          return;
        }
      }
    } else {
      UniformLocation &location = it->second;
      location.uniform = uniform;
    }
  }
}

#ifdef TEMPORARY_USE
/**
 * @brief
 *
 */
void RenderPass::Apply() {
  // Apply render states

  // Apply shader
}
#endif

/*------------------------------Material--------------------------*/

Material::Material() : Object(), _passes(), _uniforms(), _textures() {}

Material::Material(const Material &src)
    : Object(src), _passes(), _uniforms(), _textures(), _state(src._state) {
  for (auto srcPass : src._passes) {
    RenderPassPtr pass = std::make_shared<RenderPass>(*(srcPass.get()));
    _passes.emplace_back(pass);
  }

  for (auto srcUniform : src._uniforms) {
    Uniform *uniform = srcUniform.second->Clone();
    _uniforms.insert(std::map<std::string, Uniform *>::value_type(
        srcUniform.first, uniform));

    SetPassesUniform(uniform->name.c_str(), uniform);
  }

  for (auto srcTex : src._textures) {
    SetTexture(srcTex.first.c_str(), srcTex.second);
  }
}

Material::~Material() {
  _passes.clear();

  for (auto u : _uniforms) {
    SAFE_DEL(u.second);
  }
  _uniforms.clear();

  _textures.clear();
}

void Material::RemoveUniform(const char *name) {
  std::map<std::string, Uniform *>::iterator it = _uniforms.find(name);
  if (it != _uniforms.end()) {
    SAFE_DEL(it->second);
    _uniforms.erase(name);
  }
}

const Uniform *Material::GetUniform(const char *name) {
  std::map<std::string, Uniform *>::iterator it = _uniforms.find(name);
  if (it != _uniforms.end()) {
    return it->second;
  }

  return nullptr;
}

void Material::AddUniform(const char *name, Uniform *uniform) {
  _uniforms.insert(std::map<std::string, Uniform *>::value_type(name, uniform));
}

void Material::SetPassesUniform(const char *name, Uniform *uniform) {
  if (_passes.size() == 0)
    return;

  for (auto pass : _passes) {
    if (pass != nullptr) {
      pass->SetUniform(name, uniform);
    }
  }
}

void Material::ApplyRenderPass() {
  for (auto uniform : _uniforms) {
    SetPassesUniform(uniform.first.c_str(), uniform.second);
  }

  for (auto tex : _textures) {
    for (auto pass : _passes) {
      pass->SetTexture(tex.first.c_str(), tex.second);
    }
  }
}

void Material::SetMainTexture(TexturePtr maintex) {
  SetTexture("mainTex", maintex);
}

void Material::SetTexture(const char *name, TexturePtr tex) {
  std::map<std::string, TexturePtr>::iterator it = _textures.find(name);
  if (it == _textures.end()) {
    _textures.insert(std::map<std::string, TexturePtr>::value_type(name, tex));

    for (auto pass : _passes) {
      if (pass != nullptr) {
        pass->SetTexture(name, tex);
      }
    }
  } else {
    if (it->second != tex) {
      for (auto pass : _passes) {
        if (pass != nullptr) {
          pass->SetTexture(name, tex);
        }
      }
    }
    it->second = tex;
  }
}

TexturePtr Material::GetTexture(const char *name) {
  std::map<std::string, TexturePtr>::iterator it = _textures.find(name);
  if (it == _textures.end()) {
    return nullptr;
  }

  return it->second;
}

void Material::SetTextureTiling(const char *name, vec2 &tiling) {
  if (_passes.size() == 0) {
#ifdef _DEBUG

#endif
    return;
  }

  std::string sname = name;
  sname += "Tiling";
  this->SetVecUniformValue<float32, 2>(sname.c_str(), tiling);
}

void Material::SetTextureOffset(const char *name, vec2 &offset) {
  if (_passes.size() == 0) {
#ifdef _DEBUG

#endif
    return;
  }

  std::string sname = name;
  sname += "Offset";
  this->SetVecUniformValue<float32, 2>(sname.c_str(), offset);
}

} // namespace TwinkleGraphics
