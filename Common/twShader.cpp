
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
    //Todo: read program from cache first


    //read shader source
    Shader::Ptr shaders[num];
    uint32 shader_ids[num];
    for(int i = 0; i < num; i++)
    {
        shaders[i] = ReadShader(shaders_info[i]);
        shader_ids[i] = shaders[i]->GetRes().id;
    }

    //if cache not found, create new program
    ShaderProgram::Ptr program = std::make_shared<ShaderProgram>();
    if(!(program->Link(shader_ids, num)))
    {
        return nullptr;
    }

    return program;
}


/**
 * @brief Construct a new Shader:: Shader object
 * 
 * @param type 
 */
Shader::Shader(ShaderType type, const char* source)
    : Object(), _res()
{
    _res.type = (uint32)type;
    _res.id = glCreateShader(_res.type);
    glShaderSource(_res.id, 1, &source, NULL);
    glCompileShader(_res.id);
}

Shader::Shader(const Shader &copy)
    : _res(copy._res)
{

}

Shader::~Shader()
{
    std::cout<< "Shader " << _res.id  << "(hash: " << _res.hash << ")" << " deconstruct.\n";

    glDeleteShader(_res.id);
}

bool Shader::Compile()
{
    //opengl programing guide 8th source code
    GLint compiled;
    glGetShaderiv(_res.id, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
// #ifdef _DEBUG
        GLsizei len;
        glGetShaderiv(_res.id, GL_INFO_LOG_LENGTH, &len);

        GLchar *log = new GLchar[len + 1];
        glGetShaderInfoLog(_res.id, len, &len, log);
        std::cerr << "Shader compilation failed: " << log << std::endl;
        SAFE_DEL_ARR(log);
// #endif /* DEBUG */

        glDeleteShader(_res.id);
    }

    return compiled;
}

/**
 * @brief Construct a new Shader Program:: Shader Program object
 * 
 */
ShaderProgram::ShaderProgram()
    : Object(), _res()
{
    _res.id = glCreateProgram();
    _res.type = GL_PROGRAM;
}

ShaderProgram::~ShaderProgram()
{
    std::cout<< "Shader Program " << _res.id  << "(hash: " << _res.hash << ")" << " deconstruct.\n";

    glDeleteProgram(_res.id);
}

bool ShaderProgram::Link(uint32 shaders[], int num)
{
    for(int i = 0; i < num; i++)
    {
        glAttachShader(_res.id, shaders[i]);
    }

    //link program
    glLinkProgram(_res.id);

    //opengl programing guide 8th source code
    GLint linked;
    glGetProgramiv(_res.id, GL_LINK_STATUS, &linked);
    if (!linked)
    {
// #ifdef _DEBUG
        GLsizei len;
        glGetProgramiv(_res.id, GL_INFO_LOG_LENGTH, &len);

        GLchar *log = new GLchar[len + 1];
        glGetProgramInfoLog(_res.id, len, &len, log);
        std::cerr << "Shader linking failed: " << log << std::endl;
        SAFE_DEL_ARR(log);
// #endif /* DEBUG */

        for (int i = 0; i < num; i++)
        {
            glDetachShader(_res.id, shaders[i]);
        }

        glDeleteProgram(_res.id);
    }
    else
    {
        for (int i = 0; i < num; i++)
        {
            glDetachShader(_res.id, shaders[i]);
        }
    }

    return linked;
}

/**
 * @brief Construct a new Shader Resource:: Shader Resource object
 * 
 */
ShaderResource::ShaderResource()
    : Resource()
{
}

ShaderResource::~ShaderResource()
{
}


/**
 * @brief Construct a new Shader Reader:: Shader Reader object
 * 
 * @param read_info 
 */
ShaderReader::ShaderReader(ShaderReadInfo &read_info)
    : _read_info(read_info)
{
}

ShaderReader::~ShaderReader()
{
}

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
        std::cout << "Console Log: ShaderReader open shader file successed " << filename << std::endl;

        //read source
        fseek(fp, 0, SEEK_END);
        int len = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        GLchar *source = new GLchar[len + 1];
        fread(source, 1, len, fp);
        fclose(fp);

        source[len] = 0;
        const char *const_source = const_cast<const GLchar *>(source);
        Shader::Ptr shared_shader = std::make_shared<Shader>(_read_info.type, const_source);
        SAFE_DEL_ARR(source);

        if(!(shared_shader->Compile()))
        {
            return ReadResult<Shader::Ptr>(ReadResult<Shader::Ptr>::Status::FAILED);
        }

        return ReadResult<Shader::Ptr>(shared_shader, ReadResult<Shader::Ptr>::Status::SUCCESS);
    }
    else
    {
        std::cout << "Console Log: ShaderReader open shader file failed " << filename << std::endl;

#ifdef _DEBUG
        std::cerr << "Error: ShaderReader open shader file failed " << filename << std::endl;
#endif
        return ReadResult<Shader::Ptr>(ReadResult<Shader::Ptr>::Status::FAILED);
    }

    return ReadResult<Shader::Ptr>();
}



} // namespace TwinkleGraphics
