
#ifndef TW_SHADER_H
#define TW_SHADER_H

#include <string>
#include <vector>
#include <stdio.h>

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

struct ShaderReadInfo
{
    std::string filename;
    ShaderType type;
};


class Shader final : public Object
{
public:
    typedef std::shared_ptr<Shader> Ptr;
    typedef std::weak_ptr<Shader> WeakPtr;

    Shader(ShaderType type);
    virtual ~Shader();

    void SetRes(RenderResInstance& res)  {  _res = res; }
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

class ShaderReader
{
public:
    ShaderReader(ShaderReadInfo& read_info);
    ~ShaderReader();

    template <typename TPtr>
    ReadResult<TPtr> Read(const char *filename, ReaderOption *option)
    { return ReadResult<TPtr>(); }


private:
    ShaderReadInfo _read_info;
};

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

class ShaderManager
{
public:
    ShaderManager();
    ~ShaderManager();

    Shader::Ptr ReadShader(ShaderReadInfo& shader_info);
    ShaderProgram::Ptr ReadShaders(ShaderReadInfo shaders_info[], int num);

private:
    std::map<uint32, Shader::Ptr> _shaders;
    std::map<uint32, ShaderProgram::Ptr> _shader_programs;
};


} // namespace TwinkleGraphics

#endif