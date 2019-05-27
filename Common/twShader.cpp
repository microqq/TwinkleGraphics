
#include "twShader.h"

namespace TwinkleGraphics
{

ShaderManager::ShaderManager()
    :  _shader_loader(nullptr)
{
    _shader_loader = new ShaderLoader("Shader Loader");
}

ShaderManager::~ShaderManager()
{
    SAFE_DEL(_shader_loader);
}

Shader *ShaderManager::ReadShader(const char *filename)
{

}

ShaderProgram* ShaderManager::ReadShaders(const char** files)
{
    
}



Shader::Shader()
    : Object()
    , _res()
{
    _res.id = 0;
    _res.type = 0;
}

Shader::~Shader()
{}



ShaderProgram::ShaderProgram()
    : Object()
    , _res()
{
    _res.id = 0;
    _res.type = 0;
}

ShaderProgram::~ShaderProgram()
{}



ShaderResource::ShaderResource()
    : Resource()
{}

ShaderResource::~ShaderResource()
{}




ShaderLoader::ShaderLoader(const char* type)
    : ResourceLoader(type)
{}

ShaderLoader::~ShaderLoader()
{}


} // namespace TwinkleGraphics
