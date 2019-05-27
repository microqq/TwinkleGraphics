
#ifndef TW_SHADER_H
#define TW_SHADER_H

#include <string>
#include <vector>

#include "twCommon.h"
#include <glew/glew.h>

#include "twResource.hpp"

namespace TwinkleGraphics
{
class ShaderManager;
typedef Singleton<ShaderManager> ShaderManagerInst;

class ShaderManager
{
public:
    ShaderManager();
    ~ShaderManager();

    Shader* ReadShader(const char* filename);
    ShaderProgram* ReadShaders(const char** files);

private:
    ShaderLoader* _shader_loader;
};

enum class ShaderType
{
    VERTEX_SHADER = GL_VERTEX_SHADER,
    TCS_SHADER = GL_TESS_CONTROL_SHADER,
    TES_SHADER = GL_TESS_EVALUATION_SHADER,
    GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
    FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
    COMPUTE_SHADER = GL_COMPUTE_SHADER
};

class Shader : public Object
{
public:
    Shader();
    virtual ~Shader();

private:
    RenderResInstance _res;
};


class ShaderProgram : public Object
{
public:
    ShaderProgram();
    virtual ~ShaderProgram();

private:

    RenderResInstance _res;
};

class ShaderResource : public Resource
{
public:
    ShaderResource();
    virtual ~ShaderResource();
};

class ShaderLoader : public ResourceLoader
{
public:
    ShaderLoader(const char* type);
    virtual ~ShaderLoader();
};


} // namespace TwinkleGraphics

#endif