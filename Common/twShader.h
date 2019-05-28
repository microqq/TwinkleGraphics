
#ifndef TW_SHADER_H
#define TW_SHADER_H

#include <string>
#include <vector>

#include "twCommon.h"
#include <glew/glew.h>

#include "twResource.hpp"

namespace TwinkleGraphics
{
class Shader;
class ShaderProgram;
class ShaderResource;
class ShaderReader;
class ShaderManager;
typedef Singleton<ShaderManager> ShaderManagerInst;

enum class ShaderType
{
    VERTEX_SHADER = GL_VERTEX_SHADER,
    TCS_SHADER = GL_TESS_CONTROL_SHADER,
    TES_SHADER = GL_TESS_EVALUATION_SHADER,
    GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
    FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
    COMPUTE_SHADER = GL_COMPUTE_SHADER
};

struct ShaderLoadInfo
{
    ShaderType type;
    std::string filename;
};


class Shader final : public Object
{
public:
    typedef std::shared_ptr<Shader> Ptr;
    typedef std::weak_ptr<Shader> WeakPtr;

    Shader(ShaderType type);
    virtual ~Shader();

    const RenderResInstance& GetRes() { return _res; }

private:
    RenderResInstance _res;
};


class ShaderProgram final : public Object
{
public:
    typedef std::shared_ptr<ShaderProgram> Ptr;
    typedef std::weak_ptr<ShaderProgram> WeakPtr;

    ShaderProgram();
    virtual ~ShaderProgram();

    const RenderResInstance& GetRes() { return _res; }

private:
    RenderResInstance _res;
};

class ShaderProgramUse
{
public:
    ShaderProgramUse(ShaderProgram::Ptr program)
    {
        const RenderResInstance& res = program->GetRes();
        glUseProgram(res.id);
    }

    ~ShaderProgramUse()
    {
        glUseProgram(0);
    }
};


class ShaderResource final : public Resource
{
public:
    typedef std::shared_ptr<ShaderResource> Ptr;
    typedef std::weak_ptr<ShaderResource> WeakPtr;

    ShaderResource();
    virtual ~ShaderResource();
};

class ShaderReader : public ResourceReader
{
public:
    ShaderReader(const char* type);
    virtual ~ShaderReader();

};


class ShaderManager
{
public:
    ShaderManager();
    ~ShaderManager();

    Shader::Ptr ReadShader(ShaderLoadInfo& shader_info);
    ShaderProgram::Ptr ReadShaders(ShaderLoadInfo shaders_info[], int num);

private:
    std::map<uint32, Shader::Ptr> _shaders;
    std::map<uint32, ShaderProgram::Ptr> _shader_programs;
};


} // namespace TwinkleGraphics

#endif