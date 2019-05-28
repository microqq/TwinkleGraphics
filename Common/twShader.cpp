
#include "twShader.h"

namespace TwinkleGraphics
{

ShaderManager::ShaderManager()
{
    ResourceManagerInst resMgr;
    resMgr->RegisterReader("Shader Loader", new ShaderReader("Shader Loader"));
}

ShaderManager::~ShaderManager()
{
    ResourceManagerInst resMgr;
    resMgr->UnRegisterReader("Shader Loader");
}

Shader::Ptr ShaderManager::ReadShader(ShaderLoadInfo& shader_info)
{
    Shader::Ptr shader = std::make_shared<Shader>(shader_info.type);

    ResourceManagerInst resMgr;

    return shader;
}

ShaderProgram::Ptr ShaderManager::ReadShaders(ShaderLoadInfo shaders_info[], int num)
{
    //read shader source
    Shader::Ptr shaders[num];
    for(int i = 0; i < num; i++)
    {
        shaders[i] = ReadShader(shaders_info[i]);
    }

    //compile & link shader
}



Shader::Shader(ShaderType type)
    : Object()
    , _res()
{
    _res.id = 0;
    _res.type = (uint32)type;
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




ShaderReader::ShaderReader(const char* type)
    : ResourceReader(type)
{}

ShaderReader::~ShaderReader()
{}


} // namespace TwinkleGraphics
