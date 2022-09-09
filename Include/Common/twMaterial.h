
#ifndef TW_MATERIAL_H
#define TW_MATERIAL_H

#include <map>
#include <vector>

#include "twCommon.h"
#include "twRenderContext.h"
#include "twShader.h"
#include "twShaderManager.h"
#include "twTexture.h"
#include "twUniform.h"

namespace TwinkleGraphics {

/**
 * Material Construct Steps:
 * 1、Material:Construct Material
 * 2、RenderPass:Construct RenderPass
 * 3、RenderPass:Set ShaderProgram
 * 4、RenderPass:Set RenderState
 * 5、Material:Set RenderPass
 * 6、Material:Set Texture
 * 7、Material:Set Uniform
 * 8、Material:Set RenderState
 */

class RenderPass;
class Material;
class MeshRenderer;

/**
 * @brief
 *
 */
class __TWCOMExport RenderPass : public Object {
public:
  using Ptr = std::shared_ptr<RenderPass>;

  static RenderPass::Ptr CreateRenderPassInstance(ShaderOption options[],
                                                  int32 num);

  RenderPass(ShaderProgramPtr shader = nullptr);
  RenderPass(const RenderPass &src);
  virtual ~RenderPass();

  void SetCullMode(CullMode cull) {}
  void SetPolygonMode(PolygonMode polygonmode) {}

  void SetShaderProgram(ShaderProgramPtr shader) { _program = shader; }
  void SetEnable(bool enable) { _enable = enable; }

  inline bool Enable() { return _enable; }
  inline const RenderState &GetRenderState() { return _state; }
  inline const ShaderProgramPtr &GetShaderProgram() { return _program; }
  inline const std::map<std::string, TextureSlot> &GetTextureSlots() {
    return _slots;
  }
  inline const std::map<std::string, UniformLocation> &GetUniformLocations() {
    return _uniformlocations;
  }

#ifdef TEMPORARY_USE
  void Apply();
#endif

private:
  void SetMaintexture(TexturePtr maintex) { SetTexture("mainTex", maintex); }
  void SetTexture(const char *name, TexturePtr tex);
  void SetUniform(const char *name, Uniform *uniform);

private:
  std::map<std::string, TextureSlot> _slots;
  std::map<std::string, UniformLocation> _uniformlocations;
  RenderState _state;
  ShaderProgramPtr _program;

  bool _enable = true;

  friend class Material;
};

using RenderPassPtr = RenderPass::Ptr;

/**
 * @brief
 *
 */
class __TWCOMExport Material : public Object {
public:
  using Ptr = std::shared_ptr<Material>;

  Material();
  Material(const Material &src);

  virtual ~Material();
  virtual void Initialize() {}
  virtual Material::Ptr SharedClone() {
    Material::Ptr clone = std::make_shared<Material>(*this);
    return clone;
  }

  void SetCullMode(CullMode cull) {}
  void SetPolygonMode(PolygonMode polygonmode) {}

  /**
   * @brief Set the Render Pass object
   *
   * @param index
   * @param pass
   */
  void SetRenderPass(int32 index, RenderPassPtr pass) {
    int32 size = _passes.size();
    if (index < size)
      _passes[index] = pass;
  }

  /**
   * @brief
   *
   * @param pass
   */
  void AddRenderPass(RenderPassPtr pass) { _passes.push_back(pass); }

  RenderPassPtr GetRenderPass(int32 index) {
    int32 size = _passes.size();
    if (index >= 0 && index < size)
      return _passes[index];
    return nullptr;
  }

  /**
   * @brief Set the Main Texture object
   *
   * @param maintex
   */
  void SetMainTexture(TexturePtr maintex);

  /**
   * @brief Set the Texture object
   *
   * @param name
   * @param tex
   */
  void SetTexture(const char *name, TexturePtr tex);

  TexturePtr GetMainTexture() { return GetTexture("mainTex"); }
  TexturePtr GetTexture(const char *name);

  /**
   * @brief Set the Texture Tiling object
   *
   * @param name
   * @param tiling
   */
  void SetTextureTiling(const char *name, vec2 &tiling);

  /**
   * @brief Set the Texture Offset object
   *
   * @param name
   * @param offset
   */
  void SetTextureOffset(const char *name, vec2 &offset);

  /**
   * @brief Set the Simple Uniform Value object
   *
   * @tparam T: bool, float, double, int, uint,
   * @tparam N: 1, 2, 3, 4
   * @tparam Args
   * @param name
   * @param args: T t1, T t2, ...
   */
  template <class T, uint32 N, class... Args>
  void SetSimpleUniformValue(const char *name, Args &&...args) {
    SimpleUniform<T, N> *uniform = nullptr;

    std::map<std::string, Uniform *>::iterator it = _uniforms.find(name);
    if (it == _uniforms.end()) {
      uniform = new SimpleUniform<T, N>(name);
      AddUniform(name, uniform);
      SetPassesUniform(name, uniform);
    } else {
      uniform = dynamic_cast<SimpleUniform<T, N> *>(it->second);
    }

    uniform->Set(std::forward<Args>(args)...);
  }

  /**
   * @brief Set the Vec Uniform Value object
   *
   * @tparam T: bool, float, double, int, uint,
   * @tparam N: 2, 3, 4
   * @param name
   * @param value
   */
  template <class T, uint32 N>
  void SetVecUniformValue(const char *name, vec<N, T, defaultp> &value) {
    VecUniform<T, N> *uniform = nullptr;

    std::map<std::string, Uniform *>::iterator it = _uniforms.find(name);
    if (it == _uniforms.end()) {
      uniform = new VecUniform<T, N>(name);
      AddUniform(name, uniform);
      SetPassesUniform(name, uniform);
    } else {
      uniform = dynamic_cast<VecUniform<T, N> *>(it->second);
    }

    uniform->Set(value);
  }

  /**
   * @brief Set the Matrix Uniform Value object
   *
   * @tparam T: bool, float, double, int, uint,
   * @tparam Row: 2, 3, 4
   * @tparam Column: 2, 3, 4
   * @param name
   * @param value
   */
  template <class T, uint32 Row, uint32 Column>
  void SetMatrixUniformValue(const char *name,
                             mat<Row, Column, T, defaultp> &value,
                             bool transpose = false) {
    MatUniform<T, Row, Column> *uniform = nullptr;

    std::map<std::string, Uniform *>::iterator it = _uniforms.find(name);
    if (it == _uniforms.end()) {
      uniform = new MatUniform<T, Row, Column>(name);
      AddUniform(name, uniform);
      SetPassesUniform(name, uniform);
    } else {
      uniform = dynamic_cast<MatUniform<T, Row, Column> *>(it->second);
    }

    uniform->Set(value, transpose);
  }

  /**
   * @brief Get the Render State object
   *
   * @return const RenderState&
   */
  const RenderState &GetRenderState() { return _state; }

  /**
   * @brief Get the Uniform object
   *
   * @param name
   * @return const Uniform*
   */
  const Uniform *GetUniform(const char *name);

  void ApplyRenderPass();

private:
  /**
   * @brief
   *
   * @param name
   * @param uniform
   */
  void AddUniform(const char *name, Uniform *uniform);

  /**
   * @brief
   *
   * @param name
   */
  void RemoveUniform(const char *name);

  /**
   * @brief Set the Passes Uniform object
   *
   * @param name
   * @param uniform
   */
  void SetPassesUniform(const char *name, Uniform *uniform);

private:
  std::vector<RenderPassPtr> _passes;
  std::map<std::string, Uniform *> _uniforms;
  std::map<std::string, TexturePtr> _textures;
  RenderState _state;

  friend class MeshRenderer;
};

using MaterialPtr = Material::Ptr;

} // namespace TwinkleGraphics

#endif