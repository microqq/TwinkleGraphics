
#ifndef TW_SHADER_H
#define TW_SHADER_H

#include <string>
#include <vector>
#include <stdio.h>

#include <glew/glew.h>
#include "twCommon.h"

#include "twResource.h"
#include "twRenderContext.h"
#include "twTextReader.h"

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
        int32 numMacros = 0;
        char** macros = nullptr;
        bool compileImmediate = true;
    };

    typedef TextSource ShaderSource;
    typedef TextSource ShaderIncludeSource;

    class ShaderInclude : public Object
    {
    public:
        typedef std::shared_ptr<ShaderInclude> Ptr;
    };

    class Shader final : public Object
    {
    public:
        typedef std::shared_ptr<Shader> Ptr;
        typedef std::weak_ptr<Shader> WeakPtr;

        // Shader(ShaderResourceInfo::Ptr source);
        Shader(ShaderType type, ShaderSource::Ptr source = nullptr);
        // Shader(ShaderType type, const char *source);
        Shader(const Shader &);
        virtual ~Shader();

        void SetRes(RenderResInstance &res) { _res = res; }
        const RenderResInstance &GetRes() { return _res; }
        bool Compiled() { return _compiled; }

        void SetShaderSource(ShaderSource::Ptr source);
        void SetDefineMacros(const char* macros[], int length);
        void SetupCompile();
        bool Compile();
        bool CheckShadingLanguageIncludeARB();

    private:
        void InsertDefMacros(const char* defMacros[], int length);
        void ParseShaderIncludes(const char *source);

    private:
        std::vector<ShaderIncludeSource::Ptr> _includeSouces;
        ShaderSource::Ptr _source = nullptr;
        RenderResInstance _res;

        bool _setupCompile = false;
        bool _compiled = false;

        friend class ShaderManager;
    };

    class ShaderProgram final : public Object
    {
    public:
        typedef std::shared_ptr<ShaderProgram> Ptr;
        typedef std::weak_ptr<ShaderProgram> WeakPtr;

        ShaderProgram(Shader::Ptr* shaders, int32 num);
        virtual ~ShaderProgram();

        bool Link();
        void SetRes(RenderResInstance &res) { _res = res; }
        const RenderResInstance &GetRes() { return _res; }

        int32 GetActiveAtrtribsCount();
        int32 GetActiveUniformsCount();
        int32 GetActiveUniformBlocksCount();
        int32 GetActiveUniformBlock(uint32 index);
        void GetActiveUniform(uint32 index, char *name);

        int32 GetUniformLocation(const char *name) { return glGetUniformLocation(_res.id, name); }

        void GetActiveAttribute(uint32 index, char *name);
        int32 GetAttributeLocation(char *name);

    private:
        std::vector<Shader::Ptr> _shaders;
        RenderResInstance _res;
        GLint _linked = false;
        // bool _readyForLink = false;

        friend class ShaderManager;
    };

    class ShaderProgramUse
    {
    public:
        ShaderProgramUse(ShaderProgram::Ptr program)
        {
            if (program != nullptr)
            {
                const RenderResInstance &res = program->GetRes();
                glUseProgram(res.id);
            }
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
        ShaderReader(ShaderReadInfo &read_info);
        ~ShaderReader();

        template <typename TPtr>
        ReadResult<TPtr> Read(const char *filename, ReaderOption *option);

    private:
        ShaderReadInfo _readInfo;
    };

    class ShaderManager
    {
    public:
        ShaderManager();
        ~ShaderManager();

        Shader::Ptr ReadShader(ShaderReadInfo &shader_info);
        ShaderProgram::Ptr ReadShaders(ShaderReadInfo shaders_info[], int32 num);

    private:
        std::map<uint32, Shader::Ptr> _shaders;
        std::map<uint32, ShaderProgram::Ptr> _shaderPrograms;
    };

} // namespace TwinkleGraphics

#endif