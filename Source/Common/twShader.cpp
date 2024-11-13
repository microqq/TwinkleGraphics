
#include <array>
#include <regex>
#include <sstream>


#include "twConsoleLog.h"
#include "twResourceManager.h"
#include "twShader.h"
#include "twShaderManager.h"


namespace TwinkleGraphics {
ShaderOption::ShaderOption() : ReaderOption(), _optionData() {}

ShaderOption::ShaderOption(const OptionData &data)
    : ReaderOption(), _optionData() {
  _optionData.filename = data.filename;
  _optionData.type = data.type;
  _optionData.numMacros = data.numMacros;
  if (data.numMacros > 0) {
    _optionData.macros = new char *[data.numMacros];
  }
  for (int32 i = 0; i < data.numMacros; i++) {
    _optionData.macros[i] = data.macros[i];
  }
}

ShaderOption::ShaderOption(const ShaderOption &src) : ReaderOption(src) {
  _optionData.filename = src._optionData.filename;
  _optionData.type = src._optionData.type;
  _optionData.numMacros = src._optionData.numMacros;
  if (_optionData.numMacros > 0) {
    _optionData.macros = new char *[_optionData.numMacros];
  }
  for (int32 i = 0; i < _optionData.numMacros; i++) {
    _optionData.macros[i] = src._optionData.macros[i];
  }
}

const ShaderOption &ShaderOption::operator=(const ShaderOption &src) {
  _cacheHint = src._cacheHint;
  _successFuncList = src._successFuncList;
  _failedFuncList = src._failedFuncList;
  _storeHint = src._storeHint;
  _storeTime = src._storeTime;

  _optionData.filename = src._optionData.filename;
  _optionData.type = src._optionData.type;
  _optionData.numMacros = src._optionData.numMacros;
  if (_optionData.numMacros > 0) {
    _optionData.macros = new char *[_optionData.numMacros];
  }
  for (int32 i = 0; i < _optionData.numMacros; i++) {
    _optionData.macros[i] = src._optionData.macros[i];
  }

  return *this;
}

ShaderOption::~ShaderOption() { SAFE_DEL(_optionData.macros); }

Shader::Shader(ShaderType type, ShaderSourcePtr source)
    : Object(), _res(), _compiled(false) {
  _res.type = (uint32)type;
  SetShaderSource(source);
}

Shader::Shader(const Shader &src) : _res(src._res) {}

Shader::~Shader() {
  Console::LogInfo("Shader ", _res.id, "(hash: ", _res.hash,
                   ") deconstruct.\n");

  if (_res.id != GL_NONE) {
    glDeleteShader(_res.id);

    for (auto includeSource : _includeSouces) {
      if (includeSource != nullptr) {
        glDeleteNamedStringARB(includeSource->filename.length(),
                               includeSource->filename.c_str());
      }
    }
  }

  _includeSouces.clear();
  _source = nullptr;
}

void Shader::SetShaderSource(ShaderSourcePtr source) {
  if (source == nullptr) {
    return;
  }

  _source = source;
  ParseShaderIncludes(source->content.c_str());
}

void Shader::SetDefineMacros(const char *macros[], int length) {
  if (_source == nullptr) {
    return;
  }

  if (macros != nullptr) {
    InsertDefMacros(macros, length);
  }
}

void Shader::SetupCompile() {
  if (_source == nullptr || _setupCompile) {
    return;
  }

  _res.id = glCreateShader(_res.type);

#ifdef _DEBUG
<<<<<<< HEAD
  GLenum error = glGetError();
  const GLubyte *error_str = glGetString(error);
  if (error_str != nullptr) {
    Console::LogError("Shader ", error_str, "\n");
  }
#endif
=======
        GLenum error = glGetError();
        const GLubyte *error_str = glGetString(error);
#endif
    }

//     /**
//  * @brief Construct a new Shader:: Shader object
//  * 
//  * @param type 
//  */
//     Shader::Shader(ShaderType type, const char *source)
//         : Object()
//         , _res()
//         , _compiled(false)
//     {
//         _res.type = (uint32)type;
//         _res.id = glCreateShader(_res.type);

//         glShaderSource(_res.id, 1, &source, NULL);
//         glCompileShader(_res.id);

//         // test
//         {
//             _source = std::make_shared<ShaderSource>();
//             _source->content = std::string(source);
//             _source->filename = "";

//             const char* macros[] = {"#define VERTCOLOR\n", "#define VERTNORMAL\n"};
//             SetDefineMacros(macros, 2);
//         }


// #ifdef _DEBUG
//         GLenum error = glGetError();
//         const GLubyte *error_str = glGetString(error);
// #endif

//         _compiled = true;
//         _setupCompile = true;
//     }

    Shader::Shader(const Shader &copy)
        : _res(copy._res)
    {
    }

    Shader::~Shader()
    {
        Console::LogInfo("Shader: Shader ", _res.id, "(hash: ", _res.hash, ") deconstruct.\n");
>>>>>>> master

  const char *source = _source->content.c_str();
  glShaderSource(_res.id, 1, &source, NULL);

  for (auto includeSource : _includeSouces) {
    if (includeSource != nullptr) {
      // std::regex filenameRegex("/([a-zA-Z0-9_-]+/{1})*[a-zA-Z0-9_-]+.glsl");
      // std::smatch sm;
      // std::string filename = includeSource->filename;
      // std::regex_search(filename, sm, filenameRegex);

      if (!includeSource->filename.empty() && !includeSource->content.empty()) {
        // std::string includeFilename = sm.str();

        glNamedStringARB(GL_SHADER_INCLUDE_ARB, includeSource->filename.size(),
                         includeSource->filename.c_str(),
                         includeSource->content.size(),
                         includeSource->content.c_str());
#ifdef _DEBUG
        error = glGetError();
        error_str = glGetString(error);
#endif
      }
    }
  }

#ifdef _DEBUG
  error = glGetError();
  error_str = glGetString(error);
#endif

  _setupCompile = true;
}

bool Shader::Compile() {
  if (_compiled) {
    return true;
  }

  if (!_setupCompile) {
    return false;
  }

  if (!_compiled) {
    for (auto includeSource : _includeSouces) {
      if (includeSource != nullptr) {
        int32 position = includeSource->filename.find_last_of("/");
        if (position != -1) {
          std::string path = includeSource->filename.substr(0, position);
          const char *const cpath = path.c_str();
          glCompileShaderIncludeARB(_res.id, 1, &cpath, nullptr);
        }
      }
    }

    glCompileShader(_res.id);

    // release shadersouce
    // _source = nullptr;
  }

#ifdef _DEBUG
  GLenum error = glGetError();
  const GLubyte *error_str = glGetString(error);
  if (error_str != nullptr) {
    Console::LogError("Shader ", error_str, "\n");
  }
#endif

  // opengl programing guide 8th source code
  GLint compiled;
  glGetShaderiv(_res.id, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
#ifdef _DEBUG
    GLsizei len;
    glGetShaderiv(_res.id, GL_INFO_LOG_LENGTH, &len);

<<<<<<< HEAD
    GLchar *log = new GLchar[len + 1];
    glGetShaderInfoLog(_res.id, len, &len, log);
    Console::LogError("Shader ", _res.id, "  compilation failed: ", log, "\n");
    SAFE_DEL_ARR(log);
=======
            GLchar *log = new GLchar[len + 1];
            glGetShaderInfoLog(_res.id, len, &len, log);
            Console::LogError("Shader: Shader ", _res.id, "  compilation failed: ", log, "\n");
            SAFE_DEL_ARR(log);
>>>>>>> master
#endif /* DEBUG */

    glDeleteShader(_res.id);
  }

  _compiled = compiled;
  return _compiled;
}

bool Shader::CheckShadingLanguageIncludeARB() {
  return CheckSupportExtension("GL_ARB_shading_language_include");
}

void Shader::InsertDefMacros(const char *defMacros[], int length) {
  if (_source == nullptr) {
    return;
  }

  bool versionMatch = false;
  int32 position = -1;
  std::istringstream input(_source->content.c_str());
  std::regex extensionRegex("\\s*#extension\\s+");
  std::regex versionRegex("\\s*#version\\s*[0-9]*\\score\\s*");
  for (std::array<char, 256> out; input.getline(&out[0], 256);) {
    std::smatch sm;
    std::string extString(&out[0]);

    if (!versionMatch && std::regex_match(&out[0], versionRegex)) {
      position = input.tellg();
      versionMatch = true;
      continue;
    }

    if (std::regex_search(extString, sm, extensionRegex)) {
      position = input.tellg();
      continue;
    }
  }

<<<<<<< HEAD
  if (position != -1) {
    std::string preStr = _source->content.substr(0, position - 1) + "\n";
    std::string postStr =
        _source->content.substr(position, _source->content.length() - position);
    for (int32 i = 0; i < length; i++) {
      preStr = preStr + defMacros[i];
      // _source->content.insert(position - 1, )
=======
    void Shader::InsertDefMacros(const char *defMacros[], int length)
    {
        if(_source == nullptr)
        {
            return;
        }

        bool versionMatch = false;
        int32 position = -1;
        std::istringstream input(_source->content.c_str());
        for (std::array<char, 256> out; input.getline(&out[0], 256);)
        {
            std::regex extensionRegex("\\s*#extension\\s+");
            std::regex versionRegex("\\s*#version\\s*[0-9]*\\score\\s*");
            std::smatch sm;
            std::string extString(&out[0]);

            if(!versionMatch && std::regex_match(&out[0], versionRegex))
            {
                position = input.tellg();
                versionMatch = true;
                continue;
            }

            if(std::regex_search(extString, sm, extensionRegex))
            {
                position = input.tellg();
                continue;
            }
        }

        if(position != -1)
        {
            std::string preStr = _source->content.substr(0, position - 1) + "\n";
            std::string postStr = _source->content.substr(position, _source->content.length() - position);
            for(int32 i = 0; i < length; i++)
            {
                preStr = preStr + defMacros[i];
                // _source->content.insert(position - 1, )

            }
            _source->content = preStr + postStr;
        }
>>>>>>> master
    }
    _source->content = preStr + postStr;
  }
}

void Shader::ParseShaderIncludes(const char *source) {
  if (source == nullptr) {
    return;
  }

  std::istringstream input(source);
  std::regex includeRegex(
      "\\s*#include\\s*</([a-zA-Z0-9_-]+/{1})*[a-zA-Z0-9_-]+.glsl>\\s*");
  std::regex pathRegex("/([a-zA-Z0-9_-]+/{1})*[a-zA-Z0-9_-]+.glsl");
  for (std::array<char, 256> out; input.getline(&out[0], 256);) {
    bool match = std::regex_match(&out[0], includeRegex);
    if (match) {
      std::smatch sm;
      std::string pathString(&out[0]);
      std::regex_search(pathString, sm, pathRegex);

      int32 position = _source->filename.find_last_of("/");
      if (position == -1) {
        position = _source->filename.find_last_of("\\");
      }

      TextManager &textMgr = TextMgrInstance();

      std::string directory = _source->filename.substr(0, position) + "/";
      std::string textFilename{directory + sm.str()};
      ShaderIncludeSourcePtr includeSource =
          textMgr.ReadText(textFilename.c_str());

<<<<<<< HEAD
      assert(includeSource != nullptr);
      includeSource->filename = sm.str();
      ParseShaderIncludes(includeSource->content.c_str());
      _includeSouces.emplace_back(includeSource);

      // Console::LogInfo("Shader ", _res.id, " include ", sm.str(), "\n");
=======
                Console::LogInfo("ShaderInclude: Shader ", _res.id, " include ", sm.str(), "\n");
            }
        }
>>>>>>> master
    }
  }
}

ShaderProgramUse::ShaderProgramUse(ShaderProgramPtr program) {
  if (program != nullptr) {
    const RenderResourceHandle &res = program->GetRenderResource();
    glUseProgram(res.id);
  }
}

ShaderProgramUse::~ShaderProgramUse() { glUseProgram(0); }

/**
 * @brief Construct a new Shader Program:: Shader Program object
 *
 */
ShaderProgram::ShaderProgram(int32 shaderCount)
    : Object(), _shaders(), _res(), _linkShaderCount(shaderCount) {
  _res.type = GL_PROGRAM;
}

ShaderProgram::~ShaderProgram() {
  Console::LogInfo("Shader Program ", _res.id, "(hash: ", _res.hash,
                   ") deconstruct\n");

  if (_res.id != GL_NONE) {
    glDeleteProgram(_res.id);
  }
}

void ShaderProgram::AddShader(ShaderPtr shader) {
  _shaders.emplace_back(shader);
}

void ShaderProgram::ClearShader() { _shaders.clear(); }

bool ShaderProgram::Link() {
  if (_linked) {
    return true;
  }

  int32 size = _shaders.size();
  bool readyForLink = _linkShaderCount == size;
  if (!readyForLink) {
    return false;
  }

  for (auto shader : _shaders) {
    if (!shader->Compiled()) {
      shader->SetupCompile();
      shader->Compile();
    }
  }

  _res.id = glCreateProgram();

<<<<<<< HEAD
#ifdef _DEBUG
  GLenum error = glGetError();
  const GLubyte *error_str = glGetString(error);
  if (error_str != nullptr) {
    Console::LogError("ShaderProgram ", error_str, "\n");
  }
#endif
=======
    ShaderProgram::~ShaderProgram()
    {
        Console::LogInfo("Shader: Shader Program ", _res.id, "(hash: ", _res.hash, ") deconstruct\n");
>>>>>>> master

  for (int i = 0; i < _linkShaderCount; i++) {
    glAttachShader(_res.id, _shaders[i]->GetRenderResource().id);
  }

  // link program
  glLinkProgram(_res.id);

  // opengl programing guide 8th source code
  glGetProgramiv(_res.id, GL_LINK_STATUS, &_linked);
  if (!_linked) {
#ifdef _DEBUG
    GLsizei len;
    glGetProgramiv(_res.id, GL_INFO_LOG_LENGTH, &len);

<<<<<<< HEAD
    GLchar *log = new GLchar[len + 1];
    glGetProgramInfoLog(_res.id, len, &len, log);
    Console::LogError("Shader linking failed: ", log, "\n");
    SAFE_DEL_ARR(log);
=======
            GLchar *log = new GLchar[len + 1];
            glGetProgramInfoLog(_res.id, len, &len, log);
            Console::LogError("Shader: Shader linking failed: ", log, "\n");
            SAFE_DEL_ARR(log);
>>>>>>> master
#endif /* DEBUG */

    for (int i = 0; i < _linkShaderCount; i++) {
      glDetachShader(_res.id, _shaders[i]->GetRenderResource().id);
    }

    glDeleteProgram(_res.id);
  } else {
    for (int i = 0; i < _linkShaderCount; i++) {
      glDetachShader(_res.id, _shaders[i]->GetRenderResource().id);
    }
  }

  ClearShader();

  return _linked;
}

int32 ShaderProgram::GetActiveAtrtribsCount() {
  int32 count;
  glGetProgramiv(_res.id, GL_ACTIVE_ATTRIBUTES, &count);

  return count;
}

int32 ShaderProgram::GetActiveUniformBlocksCount() {
  int32 count;
  glGetProgramiv(_res.id, GL_ACTIVE_UNIFORM_BLOCKS, &count);

  return count;
}

int32 ShaderProgram::GetActiveUniformsCount() {
  int32 count;
  glGetProgramiv(_res.id, GL_ACTIVE_UNIFORMS, &count);

  return count;
}

void ShaderProgram::GetActiveAttribute(uint32 index, char *name) {
  int32 length;
  int32 size;
  GLenum type;
  glGetActiveAttrib(_res.id, index, 256, &length, &size, &type, name);
}

void ShaderProgram::GetActiveUniform(uint32 index, char *name) {
  int32 length;
  int32 size;
  GLenum type;
  glGetActiveUniform(_res.id, index, 256, &length, &size, &type, name);
}

int32 ShaderProgram::GetActiveUniformBlock(uint32 index) {
  int32 block;

  return block;
}

int32 ShaderProgram::GetAttributeLocation(char *name) {
  return glGetAttribLocation(_res.id, name);
}

/**
 * @brief Construct a new Shader Reader:: Shader Reader object
 *
 * @param read_info
 */
ShaderReader::ShaderReader() : ResourceReader() {
  // INITIALISE_READERID
}

ShaderReader::ShaderReader(ShaderOption *option) : ResourceReader() {
  if (option != nullptr) {
    if (_option != nullptr) {
      SAFE_DEL(_option);
    }
    _option = new ShaderOption(*option);
  }
}

<<<<<<< HEAD
ShaderReader::ShaderReader(ShaderProgramOption *option) : ResourceReader() {
  if (option != nullptr) {
    if (_option != nullptr) {
      SAFE_DEL(_option);
    }
    _option = new ShaderProgramOption(*option);
  }
}

ShaderReader::~ShaderReader() {}
DEFINE_READERID(ShaderReader)

/**
 * @brief
 *
 * @tparam
 * @param filename
 * @param option
 * @return ReadResult<ShaderPtr>
 */
ReadResult<Shader> ShaderReader::Read(const char *filename) {
  std::string path(filename);
  size_t len = path.find_last_of(":");
  if (len == std::string::npos) {
    len = path.length();
  }
  auto subFilename = path.substr(0, len);

  FILE *fp;
  fp = fopen(subFilename.c_str(), "rb");
  if (fp) {
    Console::LogInfo("ShaderReader open shader file ", subFilename,
                     " successed.\n");

    // opengl programing guide 8th source code
    // read source
    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    GLchar *source = new GLchar[len + 1];
    fread(source, 1, len, fp);
    fclose(fp);
    fp = nullptr;

    source[len] = 0;
    ShaderSourcePtr sourcePtr = std::make_shared<ShaderSource>();
    sourcePtr->filename = subFilename;
    sourcePtr->content = std::string(source);
    SAFE_DEL_ARR(source);

    ShaderOption *shaderOption = dynamic_cast<ShaderOption *>(_option);
    ShaderPtr sharedShader =
        std::make_shared<Shader>(shaderOption->_optionData.type, sourcePtr);
    sharedShader->SetDefineMacros(
        const_cast<const char **>(shaderOption->_optionData.macros),
        shaderOption->_optionData.numMacros);

    ReadResult<Shader> result(shared_from_this(), sharedShader,
                              ReadResult<Shader>::Status::SUCCESS);
    return result;
  } else {
#ifdef _DEBUG
    Console::LogError("ShaderReader open shader file ", subFilename,
                      " failed\n");
#endif
=======
    /**
     * @brief 
     * 
     * @tparam  
     * @param filename 
     * @param option 
     * @return ReadResult<Shader::Ptr> 
     */
    template <>
    ReadResult<Shader::Ptr> ShaderReader::Read<Shader::Ptr>(const char *filename, ReaderOption *option)
    {
        FILE *fp;
        fp = fopen(filename, "rb");
        if (fp)
        {
            Console::LogInfo("Shader: ShaderReader open shader file ", filename, " successed.\n");
>>>>>>> master

    ReadResult<Shader> result(ReadResult<Shader>::Status::FAILED);
    return result;
  }

  return ReadResult<Shader>();
}

ReadResult<Shader> ShaderReader::ReadAsync(std::string filename) {
  _asynchronize = true;
  return Read(filename.c_str());
}

ReadResult<ShaderProgram> ShaderReader::ReadProgramAsync(std::string filename) {
  using Status = typename ReadResult<ShaderProgram>::Status;

  ShaderManager &shaderMgr = ShaderMgrInstance();
  ShaderProgramOption *option = dynamic_cast<ShaderProgramOption *>(_option);
  if (option != nullptr) {
    ShaderPtr shader = nullptr;
    ShaderProgramPtr program =
        std::make_shared<ShaderProgram>(option->_numShaderOption);
    for (int i = 0, num = option->_numShaderOption; i < num; i++) {
      ShaderOption &shaderOption = option->_shaderOptions[i];
      shader = shaderMgr.ReadShader(shaderOption._optionData.filename.c_str(),
                                    &shaderOption);

<<<<<<< HEAD
      program->AddShader(shader);
=======
            if(_readInfo.compileImmediate)
            {
                if (!(sharedShader->Compile()))
                {
                    return ReadResult<Shader::Ptr>(ReadResult<Shader::Ptr>::Status::FAILED);
                }
            }

            return ReadResult<Shader::Ptr>(sharedShader, ReadResult<Shader::Ptr>::Status::SUCCESS);
        }
        else
        {
#ifdef _DEBUG
            Console::LogError("Shader: ShaderReader open shader file ", filename, " failed\n");
#endif
            return ReadResult<Shader::Ptr>(ReadResult<Shader::Ptr>::Status::FAILED);
        }

        return ReadResult<Shader::Ptr>();
>>>>>>> master
    }

    return ReadResult<ShaderProgram>(shared_from_this(), program,
                                     Status::SUCCESS);
  }

  return ReadResult<ShaderProgram>(Status::FAILED);
}
} // namespace TwinkleGraphics
