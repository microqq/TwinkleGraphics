
#ifndef TW_SHADER_H
#define TW_SHADER_H

#include <stdio.h>
#include <string>
#include <vector>

#include "twCommon.h"
#include <GL/glew.h>

#include "twRenderContext.h"
#include "twResource.h"
#include "twTextManager.h"
#include "twTextReader.h"

namespace TwinkleGraphics {
class Shader;
class ShaderProgram;
class ShaderResource;
class ShaderOption;
class ShaderReader;
class ShaderManager;

enum class ShaderType {
  VERTEX_SHADER = GL_VERTEX_SHADER,
  TCS_SHADER = GL_TESS_CONTROL_SHADER,
  TES_SHADER = GL_TESS_EVALUATION_SHADER,
  GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
  FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
  COMPUTE_SHADER = GL_COMPUTE_SHADER
};

using ShaderSource = TextSource;
using ShaderIncludeSource = TextSource;
using ShaderSourcePtr = ShaderSource::Ptr;
using ShaderIncludeSourcePtr = ShaderIncludeSource::Ptr;

class Shader final : public Object {
public:
  using Ptr = std::shared_ptr<Shader>;
  using WeakPtr = std::weak_ptr<Shader>;

  // Shader(ShaderResourceInfo::Ptr source);
  Shader(ShaderType type, ShaderSourcePtr source = nullptr);
  // Shader(ShaderType type, const char *source);
  Shader(const Shader &);
  virtual ~Shader();

  void SetRenderResource(RenderResourceHandle &res) { _res = res; }
  const RenderResourceHandle &GetRenderResource() { return _res; }
  bool Compiled() { return _compiled; }

  ShaderSourcePtr GetShaderSource() { return _source; }
  void SetShaderSource(ShaderSourcePtr source);
  void SetDefineMacros(const char *macros[], int length);
  void SetupCompile();
  bool Compile();
  bool CheckShadingLanguageIncludeARB();

private:
  void InsertDefMacros(const char *defMacros[], int length);
  void ParseShaderIncludes(const char *source);

private:
  std::vector<ShaderIncludeSourcePtr> _includeSouces;
  ShaderSourcePtr _source = nullptr;
  RenderResourceHandle _res;

  bool _setupCompile = false;
  bool _compiled = false;

  friend class ShaderManager;
};

using ShaderPtr = Shader::Ptr;

class ShaderProgram final : public Object {
public:
  using Ptr = std::shared_ptr<ShaderProgram>;
  using WeakPtr = std::weak_ptr<ShaderProgram>;

  ShaderProgram(int32 shaderCount);
  virtual ~ShaderProgram();

  void AddShader(ShaderPtr shader);
  void ClearShader();
  bool Link();
  void SetRenderResource(RenderResourceHandle &res) { _res = res; }
  const RenderResourceHandle &GetRenderResource() { return _res; }

  int32 GetActiveAtrtribsCount();
  int32 GetActiveUniformsCount();
  int32 GetActiveUniformBlocksCount();
  int32 GetActiveUniformBlock(uint32 index);
  void GetActiveUniform(uint32 index, char *name);

  int32 GetUniformLocation(const char *name) {
    return glGetUniformLocation(_res.id, name);
  }

  void GetActiveAttribute(uint32 index, char *name);
  int32 GetAttributeLocation(char *name);

private:
  std::vector<ShaderPtr> _shaders;
  RenderResourceHandle _res;
  int32 _linkShaderCount = 0;
  GLint _linked = false;

  friend class ShaderManager;
};

using ShaderProgramPtr = ShaderProgram::Ptr;

class __TWCOMExport ShaderOption final : public ReaderOption {
public:
  struct OptionData {
    std::string filename;
    ShaderType type;
    int32 numMacros = 0;
    char **macros = nullptr;
  };

  ShaderOption();
  ShaderOption(const OptionData &data);
  ShaderOption(const ShaderOption &src);
  const ShaderOption &operator=(const ShaderOption &src);
  virtual ~ShaderOption();

private:
  OptionData _optionData;

  friend class ShaderReader;
  friend class ShaderManager;
};

class __TWCOMExport ShaderProgramOption final : public ReaderOption {
public:
  ShaderProgramOption(ShaderOption *options, int num)
      : ReaderOption(), _macros() {
    FillShaderOptions(options, num);
  }

  ShaderProgramOption(const ShaderProgramOption &src)
      : ReaderOption(src), _macros(src._macros) {
    FillShaderOptions(src._shaderOptions, src._numShaderOption);
  }

  const ShaderProgramOption &operator=(const ShaderProgramOption &src) = delete;

  virtual ~ShaderProgramOption() { SAFE_DEL_ARR(_shaderOptions); }

  void SetMacros(std::string macros) { _macros = macros; }

private:
  void FillShaderOptions(ShaderOption *options, int num) {
    if (options == nullptr)
      return;

    SAFE_DEL_ARR(_shaderOptions);

    _shaderOptions = new ShaderOption[num];
    _numShaderOption = num;

    for (int i = 0; i < num; i++) {
      _shaderOptions[i] = options[i];
    }
  }

private:
  std::string _macros;
  ShaderOption *_shaderOptions = nullptr;
  int _numShaderOption;

  friend class ShaderReader;
  friend class ShaderManager;
};

class __TWCOMExport ShaderProgramUse {
public:
  ShaderProgramUse(ShaderProgramPtr program);
  ~ShaderProgramUse();
};

class __TWCOMExport ShaderReader : public ResourceReader,
                                   public Reference<ShaderReader>,
                                   public INonCopyable {
public:
  using Ptr = std::shared_ptr<ShaderReader>;

  ShaderReader();
  ShaderReader(ShaderOption *option);
  ShaderReader(ShaderProgramOption *option);
  virtual ~ShaderReader();

  ReadResult<Shader> Read(const char *filename);
  ReadResult<Shader> ReadAsync(std::string filename);
  ReadResult<ShaderProgram> ReadProgramAsync(std::string filename);

  void SetOption(ShaderOption *option) {
    if (option == nullptr)
      return;

    if (_option != nullptr) {
      SAFE_DEL(_option);
    }
    _option = new ShaderOption(*option);
  }

  void SetOption(ShaderProgramOption *option) {
    if (option == nullptr)
      return;

    if (_option != nullptr) {
      SAFE_DEL(_option);
    }
    _option = new ShaderProgramOption(*option);
  }

  DECLARE_READERID;

private:
};

} // namespace TwinkleGraphics

#endif