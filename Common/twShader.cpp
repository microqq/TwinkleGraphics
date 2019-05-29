
#include "twShader.h"

namespace TwinkleGraphics
{

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
}

Shader::Ptr ShaderManager::ReadShader(ShaderReadInfo& shader_info)
{
    ResourceManagerInst resMgr;
    ReadResult<Shader::Ptr> result = resMgr->Read<ShaderReader, Shader::Ptr>(shader_info.filename.c_str(), new ReaderOption, shader_info);
    Shader::Ptr shared_shader = result.GetSharedObject();

    return shared_shader;
}

ShaderProgram::Ptr ShaderManager::ReadShaders(ShaderReadInfo shaders_info[], int num)
{
    //read shader source
    Shader::Ptr shaders[num];
    for(int i = 0; i < num; i++)
    {
        shaders[i] = ReadShader(shaders_info[i]);
    }

    //compile & link shader

    ShaderProgram::Ptr program = std::make_shared<ShaderProgram>();
    return program;
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




ShaderReader::ShaderReader(ShaderReadInfo& read_info)
    : _read_info(read_info)
{}

ShaderReader::~ShaderReader()
{}


} // namespace TwinkleGraphics
