
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


    //if cache not found, create new program
    ShaderProgram::Ptr program = std::make_shared<ShaderProgram>();
    program->_res.type = GL_PROGRAM;
    program->_res.id = glCreateProgram();

    //read shader source
    Shader::Ptr shaders[num];
    for(int i = 0; i < num; i++)
    {
        shaders[i] = ReadShader(shaders_info[i]);

        glAttachShader(program->_res.id, shaders[i]->_res.id);
    }

    //link program
    glLinkProgram(program->_res.id);

    //opengl programing guide 8th source code
    GLint linked;
    glGetProgramiv(program->_res.id, GL_LINK_STATUS, &linked);
    if (!linked)
    {
#ifdef _DEBUG
        GLsizei len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

        GLchar *log = new GLchar[len + 1];
        glGetProgramInfoLog(program->_res.id, len, &len, log);
        std::cerr << "Shader linking failed: " << log << std::endl;
        SAFE_DEL_ARR(log);
#endif /* DEBUG */

        glDeleteProgram(program->_res.id);

        return nullptr;
    }

    return program;
}


/**
 * @brief Construct a new Shader:: Shader object
 * 
 * @param type 
 */
Shader::Shader(ShaderType type)
    : Object(), _res()
{
    _res.id = 0;
    _res.type = (uint32)type;
}

Shader::~Shader()
{
}



/**
 * @brief Construct a new Shader Program:: Shader Program object
 * 
 */
ShaderProgram::ShaderProgram()
    : Object(), _res()
{
    _res.id = 0;
    _res.type = 0;
}

ShaderProgram::~ShaderProgram()
{
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

        Shader::Ptr shared_shader = std::make_shared<Shader>(_read_info.type);

        //read source
        fseek(fp, 0, SEEK_END);
        int len = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        GLchar *source = new GLchar[len + 1];
        fread(source, 1, len, fp);
        fclose(fp);

        source[len] = 0;
        const char *const_source = const_cast<const GLchar *>(source);

        RenderResInstance res;
        res.id = glCreateShader((GLenum)(_read_info.type));
        // glShaderSource(uint, int, char* const*, int*)
        glShaderSource(res.id, 1, &const_source, NULL);
        glCompileShader(res.id);

        //opengl programing guide 8th source code
        GLint compiled;
        glGetShaderiv(res.id, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
#ifdef _DEBUG
            GLsizei len;
            glGetShaderiv(res.id, GL_INFO_LOG_LENGTH, &len);

            GLchar *log = new GLchar[len + 1];
            glGetShaderInfoLog(res.id, len, &len, log);
            std::cerr << "Shader compilation failed: " << log << std::endl;
            SAFE_DEL_ARR(log);
#endif /* DEBUG */

            glDeleteShader(res.id);

            return ReadResult<Shader::Ptr>(ReadResult<Shader::Ptr>::Status::FAILED);
        }

        res.type = (uint32)(_read_info.type);
        shared_shader->SetRes(res);

        SAFE_DEL_ARR(source);

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
