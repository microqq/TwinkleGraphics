#ifndef TW_MATERIALINSTANCE_H
#define TW_MATERIALINSTANCE_H

#include "twMaterial.h"

namespace TwinkleGraphics {

class __TWAPI StandardMaterial : public Material {
public:
  StandardMaterial(std::string vertLayoutMacros)
      : Material(), _vertLayoutMacros(vertLayoutMacros) {}
  StandardMaterial(const StandardMaterial &src)
      : Material(src), _vertLayoutMacros(src._vertLayoutMacros) {}
  virtual ~StandardMaterial() {}

  void AddTexDefineMacros(std::string macro) { _vertLayoutMacros += macro; }

protected:
  virtual void Initialize() override {
    ShaderManager &shaderMgr = ShaderMgrInstance();
    char *vertMarco = const_cast<char *>(_vertLayoutMacros.c_str());
    char *vertMacros[] = {vertMarco};
    ShaderOption options[] = {
        ShaderOption::OptionData{std::string("Assets/Shaders/standard.vert"),
                                 ShaderType::VERTEX_SHADER, 1, vertMacros},
        ShaderOption::OptionData{std::string("Assets/Shaders/standard.frag"),
                                 ShaderType::FRAGMENT_SHADER, 1, vertMacros}};

    ShaderProgramOption programOption(options, 2);
    programOption.SetMacros(_vertLayoutMacros);
    programOption.AddSuccessFunc(-1, this,
                                 &StandardMaterial::OnMaterialSuccess);
    shaderMgr.ReadShadersAsync(&programOption, 2);
  }

  virtual MaterialPtr SharedClone() override {
    MaterialPtr clone = std::make_shared<StandardMaterial>(*this);
    return clone;
  }

  void OnMaterialSuccess(ObjectPtr obj) {
    if (obj == nullptr || this->IsValid())
      return;

    ShaderProgramPtr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
    if (program != nullptr) {
      RenderPassPtr pass = std::make_shared<RenderPass>(program);
      this->AddRenderPass(pass);
      this->ApplyRenderPass();

      vec4 tintColor(1.0f, 1.0f, 1.0f, 1.0f);
      this->SetVecUniformValue<float32, 4>("tintColor", tintColor);

      this->SetValid(true);
    }
  }

protected:
  std::string _vertLayoutMacros;
};

using StandardMaterialPtr = std::shared_ptr<StandardMaterial>;

class __TWAPI BasicGeomMaterial : public Material {
public:
  BasicGeomMaterial() : Material() {}
  BasicGeomMaterial(const BasicGeomMaterial &src) : Material(src) {}
  virtual ~BasicGeomMaterial() {}

protected:
  virtual void Initialize() override {
    ShaderManager &shaderMgr = ShaderMgrInstance();

    ShaderOption options[] = {
        ShaderOption::OptionData{
            std::string("Assets/Shaders/basicGeometry.vert"),
            ShaderType::VERTEX_SHADER},
        ShaderOption::OptionData{
            std::string("Assets/Shaders/basicGeometry.frag"),
            ShaderType::FRAGMENT_SHADER}};

    ShaderProgramOption programOption(options, 2);
    programOption.AddSuccessFunc(-1, this,
                                 &BasicGeomMaterial::OnMaterialSuccess);
    shaderMgr.ReadShadersAsync(&programOption, 2);
  }

  virtual MaterialPtr SharedClone() override {
    MaterialPtr clone = std::make_shared<BasicGeomMaterial>(*this);
    return clone;
  }

  void OnMaterialSuccess(ObjectPtr obj) {
    if (obj == nullptr)
      return;

    ShaderProgramPtr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
    if (program != nullptr) {
      RenderPassPtr pass = std::make_shared<RenderPass>(program);
      this->AddRenderPass(pass);
      this->ApplyRenderPass();

      vec4 tintColor(1.0f, 1.0f, 1.0f, 1.0f);
      this->SetVecUniformValue<float32, 4>("tintColor", tintColor);

      this->SetValid(true);
    }
  }
};

using BasicGeomMaterialPtr = std::shared_ptr<BasicGeomMaterial>;

class __TWAPI SpriteMaterial : public Material {
public:
  SpriteMaterial() : Material() {}
  SpriteMaterial(const SpriteMaterial &src) : Material(src) {}
  virtual ~SpriteMaterial() {}

protected:
  virtual void Initialize() override {
    ShaderManager &shaderMgr = ShaderMgrInstance();

    char *vertMacros[1] = {const_cast<char *>(VertexLayoutDefines[3])};
    ShaderOption options[] = {
        ShaderOption::OptionData{std::string("Assets/Shaders/sprite.vert"),
                                 ShaderType::VERTEX_SHADER, 1, vertMacros},
        ShaderOption::OptionData{std::string("Assets/Shaders/sprite.frag"),
                                 ShaderType::FRAGMENT_SHADER}};

    ShaderProgramOption programOption(options, 2);
    programOption.AddSuccessFunc(-1, this, &SpriteMaterial::OnMaterialSuccess);
    shaderMgr.ReadShadersAsync(&programOption, 2);
  }

  virtual MaterialPtr SharedClone() override {
    MaterialPtr clone = std::make_shared<SpriteMaterial>(*this);
    return clone;
  }

  void OnMaterialSuccess(ObjectPtr obj) {
    if (obj == nullptr)
      return;

    ShaderProgramPtr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
    if (program != nullptr) {
      RenderPassPtr pass = std::make_shared<RenderPass>(program);
      this->AddRenderPass(pass);
      this->ApplyRenderPass();

      vec4 tintColor(1.0f, 1.0f, 1.0f, 1.0f);
      bvec2 flip(false, false);
      this->SetVecUniformValue<bool, 2>("flip", flip);
      this->SetVecUniformValue<float32, 4>("tintColor", tintColor);
      vec2 tiling(1.0f, 1.0f), offset(0.0f, 0.0f);
      this->SetTextureTiling("mainTex", tiling);
      this->SetTextureOffset("mainTex", offset);

      this->SetValid(true);
    }
  }
};

using SpriteMaterialPtr = std::shared_ptr<SpriteMaterial>;

class __TWAPI Sprite1DMaterial : public Material {
public:
  Sprite1DMaterial() : Material() {}
  Sprite1DMaterial(const Sprite1DMaterial &src) : Material(src) {}
  virtual ~Sprite1DMaterial() {}

protected:
  virtual void Initialize() override {
    ShaderManager &shaderMgr = ShaderMgrInstance();

    char *vertMacros[1] = {const_cast<char *>(VertexLayoutDefines[3])};
    ShaderOption options[] = {
        ShaderOption::OptionData{std::string("Assets/Shaders/sprite.vert"),
                                 ShaderType::VERTEX_SHADER, 1, vertMacros},
        ShaderOption::OptionData{std::string("Assets/Shaders/sprite1D.frag"),
                                 ShaderType::FRAGMENT_SHADER}};

    ShaderProgramOption programOption(options, 2);
    programOption.AddSuccessFunc(-1, this,
                                 &Sprite1DMaterial::OnMaterialSuccess);
    shaderMgr.ReadShadersAsync(&programOption, 2);
  }

  virtual MaterialPtr SharedClone() override {
    MaterialPtr clone = std::make_shared<Sprite1DMaterial>(*this);
    return clone;
  }

  void OnMaterialSuccess(ObjectPtr obj) {
    if (obj == nullptr)
      return;

    ShaderProgramPtr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
    if (program != nullptr) {
      RenderPassPtr pass = std::make_shared<RenderPass>(program);
      this->AddRenderPass(pass);
      this->ApplyRenderPass();

      vec4 tintColor(1.0f, 1.0f, 1.0f, 1.0f);
      bvec2 flip(false, false);
      this->SetVecUniformValue<bool, 2>("flip", flip);
      this->SetVecUniformValue<float32, 4>("tintColor", tintColor);
      vec2 tiling(1.0f, 1.0f), offset(0.0f, 0.0f);
      this->SetTextureTiling("mainTex", tiling);
      this->SetTextureOffset("mainTex", offset);

      this->SetValid(true);
    }
  }
};

using Sprite1DMaterialPtr = std::shared_ptr<Sprite1DMaterial>;

class __TWAPI LineMaterial : public Material {
public:
  LineMaterial() : Material() {}
  LineMaterial(const LineMaterial &src) : Material(src) {}
  virtual ~LineMaterial() {}

protected:
  virtual void Initialize() override {
    ShaderManager &shaderMgr = ShaderMgrInstance();

    ShaderOption options[] = {
        ShaderOption::OptionData{std::string("Assets/Shaders/line.vert"),
                                 ShaderType::VERTEX_SHADER},
        ShaderOption::OptionData{std::string("Assets/Shaders/line.geom"),
                                 ShaderType::GEOMETRY_SHADER},
        ShaderOption::OptionData{std::string("Assets/Shaders/line.frag"),
                                 ShaderType::FRAGMENT_SHADER}};

    ShaderProgramOption programOption(options, 3);
    programOption.AddSuccessFunc(-1, this, &LineMaterial::OnMaterialSuccess);
    shaderMgr.ReadShadersAsync(&programOption, 3);
  }

  virtual MaterialPtr SharedClone() override {
    MaterialPtr clone = std::make_shared<LineMaterial>(*this);
    return clone;
  }

  void OnMaterialSuccess(ObjectPtr obj) {
    if (obj == nullptr)
      return;

    ShaderProgramPtr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
    if (program != nullptr) {
      RenderPassPtr pass = std::make_shared<RenderPass>(program);
      this->AddRenderPass(pass);
      this->ApplyRenderPass();

      this->SetValid(true);
    }
  }
};

using LineMaterialPtr = std::shared_ptr<LineMaterial>;

class __TWAPI InfinitePlaneMaterial : public Material {
public:
  InfinitePlaneMaterial() : Material() {}
  InfinitePlaneMaterial(const InfinitePlaneMaterial &src) : Material(src) {}
  virtual ~InfinitePlaneMaterial() {}

protected:
  virtual void Initialize() override {
    ShaderManager &shaderMgr = ShaderMgrInstance();

    ShaderOption options[] = {
        ShaderOption::OptionData{
            std::string("Assets/Shaders/infinitePlane.vert"),
            ShaderType::VERTEX_SHADER},
        ShaderOption::OptionData{
            std::string("Assets/Shaders/infinitePlane.frag"),
            ShaderType::FRAGMENT_SHADER}};

    ShaderProgramOption programOption(options, 2);
    programOption.AddSuccessFunc(-1, this,
                                 &InfinitePlaneMaterial::OnMaterialSuccess);
    shaderMgr.ReadShadersAsync(&programOption, 2);
  }

  virtual MaterialPtr SharedClone() override {
    MaterialPtr clone = std::make_shared<InfinitePlaneMaterial>(*this);
    return clone;
  }

  void OnMaterialSuccess(ObjectPtr obj) {
    if (obj == nullptr)
      return;

    ShaderProgramPtr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
    if (program != nullptr) {
      RenderPassPtr pass = std::make_shared<RenderPass>(program);
      this->AddRenderPass(pass);
      this->ApplyRenderPass();

      this->SetValid(true);
    }
  }
};

using InfinitePlaneMaterialPtr = std::shared_ptr<InfinitePlaneMaterial>;

class __TWAPI VolumnQuadMaterial : public Material {
public:
  VolumnQuadMaterial() : Material() {}
  VolumnQuadMaterial(const VolumnQuadMaterial &src) : Material(src) {}
  virtual ~VolumnQuadMaterial() {}

protected:
  virtual void Initialize() override {
    ShaderManager &shaderMgr = ShaderMgrInstance();

    char *vertMacros[1] = {const_cast<char *>(VertexLayoutDefines[3])};
    ShaderOption options[] = {
        ShaderOption::OptionData{std::string("Assets/Shaders/volumnQuad.vert"),
                                 ShaderType::VERTEX_SHADER, 1, vertMacros},
        ShaderOption::OptionData{std::string("Assets/Shaders/volumnQuad.frag"),
                                 ShaderType::FRAGMENT_SHADER}};

    ShaderProgramOption programOption(options, 2);
    programOption.AddSuccessFunc(-1, this,
                                 &VolumnQuadMaterial::OnMaterialSuccess);
    shaderMgr.ReadShadersAsync(&programOption, 2);
  }

  virtual MaterialPtr SharedClone() override {
    MaterialPtr clone = std::make_shared<VolumnQuadMaterial>(*this);
    return clone;
  }

  void OnMaterialSuccess(ObjectPtr obj) {
    if (obj == nullptr)
      return;

    ShaderProgramPtr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
    if (program != nullptr) {
      RenderPassPtr pass = std::make_shared<RenderPass>(program);
      this->AddRenderPass(pass);
      this->ApplyRenderPass();

      vec4 tintColor(1.0f, 1.0f, 1.0f, 1.0f);
      bvec2 flip(false, false);
      this->SetVecUniformValue<bool, 2>("flip", flip);
      this->SetVecUniformValue<float32, 4>("tintColor", tintColor);
      vec2 tiling(1.0f, 1.0f), offset(0.0f, 0.0f);
      this->SetTextureTiling("mainTex", tiling);
      this->SetTextureOffset("mainTex", offset);

      this->SetValid(true);
    }
  }
};

using VolumnQuadMaterialPtr = std::shared_ptr<VolumnQuadMaterial>;

class __TWAPI CubeMaterial : public Material {
public:
  CubeMaterial() : Material() {}
  CubeMaterial(const CubeMaterial &src) : Material(src) {}
  virtual ~CubeMaterial() {}

protected:
  virtual void Initialize() override {
    ShaderManager &shaderMgr = ShaderMgrInstance();

    ShaderOption options[] = {
        ShaderOption::OptionData{std::string("Assets/Shaders/cube.vert"),
                                 ShaderType::VERTEX_SHADER},
        ShaderOption::OptionData{std::string("Assets/Shaders/cube.frag"),
                                 ShaderType::FRAGMENT_SHADER}};

    ShaderProgramOption programOption(options, 2);
    programOption.AddSuccessFunc(-1, this, &CubeMaterial::OnMaterialSuccess);
    shaderMgr.ReadShadersAsync(&programOption, 2);
  }

  virtual MaterialPtr SharedClone() override {
    MaterialPtr clone = std::make_shared<CubeMaterial>(*this);
    return clone;
  }

  void OnMaterialSuccess(ObjectPtr obj) {
    if (obj == nullptr)
      return;

    ShaderProgramPtr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
    if (program != nullptr) {
      RenderPassPtr pass = std::make_shared<RenderPass>(program);
      this->AddRenderPass(pass);
      this->ApplyRenderPass();

      vec4 tintColor(1.0f, 1.0f, 1.0f, 1.0f);
      this->SetVecUniformValue<float32, 4>("tintColor", tintColor);

      this->SetValid(true);
    }
  }
};

using CubeMaterialPtr = std::shared_ptr<CubeMaterial>;

class __TWAPI SphereMaterial : public Material {
public:
  SphereMaterial() : Material() {}
  SphereMaterial(const SphereMaterial &src) : Material(src) {}
  virtual ~SphereMaterial() {}

protected:
  virtual void Initialize() override {
    ShaderManager &shaderMgr = ShaderMgrInstance();

    ShaderOption options[] = {
        ShaderOption::OptionData{std::string("Assets/Shaders/sphere.vert"),
                                 ShaderType::VERTEX_SHADER},
        ShaderOption::OptionData{std::string("Assets/Shaders/sphere.frag"),
                                 ShaderType::FRAGMENT_SHADER}};

    ShaderProgramOption programOption(options, 2);
    programOption.AddSuccessFunc(-1, this, &SphereMaterial::OnMaterialSuccess);
    shaderMgr.ReadShadersAsync(&programOption, 2);
  }

  virtual MaterialPtr SharedClone() override {
    MaterialPtr clone = std::make_shared<SphereMaterial>(*this);
    return clone;
  }

  void OnMaterialSuccess(ObjectPtr obj) {
    if (obj == nullptr)
      return;

    ShaderProgramPtr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
    if (program != nullptr) {
      RenderPassPtr pass = std::make_shared<RenderPass>(program);
      this->AddRenderPass(pass);
      this->ApplyRenderPass();

      vec4 tintColor(1.0f, 1.0f, 1.0f, 1.0f);
      this->SetVecUniformValue<float32, 4>("tintColor", tintColor);

      this->SetValid(true);
    }
  }
};

using SphereMaterialPtr = std::shared_ptr<SphereMaterial>;

class __TWAPI SkyboxMaterial : public Material {
public:
  SkyboxMaterial() : Material() {}
  SkyboxMaterial(const SkyboxMaterial &src) : Material(src) {}
  virtual ~SkyboxMaterial() {}

protected:
  virtual void Initialize() override {
    ShaderManager &shaderMgr = ShaderMgrInstance();

    ShaderOption options[] = {
        ShaderOption::OptionData{std::string("Assets/Shaders/skybox.vert"),
                                 ShaderType::VERTEX_SHADER},
        ShaderOption::OptionData{std::string("Assets/Shaders/skybox.frag"),
                                 ShaderType::FRAGMENT_SHADER}};

    ShaderProgramOption programOption(options, 2);
    programOption.AddSuccessFunc(-1, this, &SkyboxMaterial::OnMaterialSuccess);
    shaderMgr.ReadShadersAsync(&programOption, 2);
  }

  virtual MaterialPtr SharedClone() override {
    MaterialPtr clone = std::make_shared<SkyboxMaterial>(*this);
    return clone;
  }

  void OnMaterialSuccess(ObjectPtr obj) {
    if (obj == nullptr)
      return;

    ShaderProgramPtr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
    if (program != nullptr) {
      RenderPassPtr pass = std::make_shared<RenderPass>(program);
      this->AddRenderPass(pass);
      this->ApplyRenderPass();

      vec4 tintColor(1.0f, 1.0f, 1.0f, 1.0f);
      bvec2 flip(false, false);
      this->SetVecUniformValue<bool, 2>("flip", flip);
      this->SetVecUniformValue<float32, 4>("tintColor", tintColor);
      vec2 tiling(1.0f, 1.0f), offset(0.0f, 0.0f);
      this->SetTextureTiling("mainTex", tiling);
      this->SetTextureOffset("mainTex", offset);

      this->SetValid(true);
    }
  }
};

using SkyboxMaterialPtr = std::shared_ptr<SkyboxMaterial>;

class __TWAPI ProjectionMappingMaterial : public Material {
public:
  ProjectionMappingMaterial() : Material() {}
  ProjectionMappingMaterial(const ProjectionMappingMaterial &src)
      : Material(src) {}
  virtual ~ProjectionMappingMaterial() {}

protected:
  virtual void Initialize() override {
    ShaderManager &shaderMgr = ShaderMgrInstance();

    ShaderOption options[] = {
        ShaderOption::OptionData{
            std::string("Assets/Shaders/projectionTexture.vert"),
            ShaderType::VERTEX_SHADER},
        ShaderOption::OptionData{
            std::string("Assets/Shaders/projectionTexture.frag"),
            ShaderType::FRAGMENT_SHADER}};

    ShaderProgramOption programOption(options, 2);
    programOption.AddSuccessFunc(-1, this,
                                 &ProjectionMappingMaterial::OnMaterialSuccess);
    shaderMgr.ReadShadersAsync(&programOption, 2);
  }

  virtual MaterialPtr SharedClone() override {
    MaterialPtr clone = std::make_shared<ProjectionMappingMaterial>(*this);
    return clone;
  }

  void OnMaterialSuccess(ObjectPtr obj) {
    if (obj == nullptr)
      return;

    ShaderProgramPtr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
    if (program != nullptr) {
      RenderPassPtr pass = std::make_shared<RenderPass>(program);
      this->AddRenderPass(pass);
      this->ApplyRenderPass();

      vec4 tintColor(1.0f, 1.0f, 1.0f, 1.0f);
      this->SetVecUniformValue<float32, 4>("tintColor", tintColor);

      this->SetValid(true);
    }
  }
};

using ProjectionMappingMaterialPtr = std::shared_ptr<ProjectionMappingMaterial>;

class __TWAPI ScreenQuadMaterial : public Material {
public:
  ScreenQuadMaterial() : Material() {}
  ScreenQuadMaterial(const ScreenQuadMaterial &src) : Material(src) {}
  virtual ~ScreenQuadMaterial() {}

protected:
  virtual void Initialize() override {
    ShaderManager &shaderMgr = ShaderMgrInstance();

    char *vertMacros[1] = {const_cast<char *>(VertexLayoutDefines[3])};
    ShaderOption options[] = {
        ShaderOption::OptionData{std::string("Assets/Shaders/screenquad.vert"),
                                 ShaderType::VERTEX_SHADER, 1, vertMacros},
        ShaderOption::OptionData{std::string("Assets/Shaders/screenquad.frag"),
                                 ShaderType::FRAGMENT_SHADER}};

    ShaderProgramOption programOption(options, 2);
    programOption.AddSuccessFunc(-1, this,
                                 &ScreenQuadMaterial::OnMaterialSuccess);
    shaderMgr.ReadShadersAsync(&programOption, 2);
  }

  virtual MaterialPtr SharedClone() override {
    MaterialPtr clone = std::make_shared<ScreenQuadMaterial>(*this);
    return clone;
  }

  void OnMaterialSuccess(ObjectPtr obj) {
    if (obj == nullptr)
      return;

    ShaderProgramPtr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
    if (program != nullptr) {
      RenderPassPtr pass = std::make_shared<RenderPass>(program);
      this->AddRenderPass(pass);
      this->ApplyRenderPass();

      this->SetValid(true);
    }
  }
};

using ScreenQuadMaterialPtr = std::shared_ptr<ScreenQuadMaterial>;

class __TWAPI MSAAResolveMaterial : public Material {
public:
  MSAAResolveMaterial() : Material() {}
  MSAAResolveMaterial(const MSAAResolveMaterial &src) : Material(src) {}
  virtual ~MSAAResolveMaterial() {}

protected:
  virtual void Initialize() override {
    ShaderManager &shaderMgr = ShaderMgrInstance();

    char *vertMacros[1] = {const_cast<char *>(VertexLayoutDefines[3])};
    ShaderOption options[] = {
        ShaderOption::OptionData{std::string("Assets/Shaders/screenquad.vert"),
                                 ShaderType::VERTEX_SHADER, 1, vertMacros},
        ShaderOption::OptionData{std::string("Assets/Shaders/msaaResolve.frag"),
                                 ShaderType::FRAGMENT_SHADER}};

    ShaderProgramOption programOption(options, 2);
    programOption.AddSuccessFunc(-1, this,
                                 &MSAAResolveMaterial::OnMaterialSuccess);
    shaderMgr.ReadShadersAsync(&programOption, 2);
  }

  virtual MaterialPtr SharedClone() override {
    MaterialPtr clone = std::make_shared<MSAAResolveMaterial>(*this);
    return clone;
  }

  void OnMaterialSuccess(ObjectPtr obj) {
    if (obj == nullptr)
      return;

    ShaderProgramPtr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
    if (program != nullptr) {
      RenderPassPtr pass = std::make_shared<RenderPass>(program);
      this->AddRenderPass(pass);
      this->ApplyRenderPass();

      this->SetValid(true);
    }
  }
};

using MSAAResolveMaterialPtr = std::shared_ptr<MSAAResolveMaterial>;

} // namespace TwinkleGraphics

#endif