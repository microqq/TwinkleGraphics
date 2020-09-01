
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
    class ShaderOption;
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

    class ShaderOption final : public ReaderOption
    {
    public:
        struct OptionData
        {
            std::string filename;
            ShaderType type;
            int32 numMacros = 0;
            char **macros = nullptr;
        };
           
        ShaderOption(const OptionData& data)
            : ReaderOption()
            , optionData(data)
        {
            optionData.macros = new char*[data.numMacros];
            for(uint i = 0; i < data.numMacros; i++)
            {
                optionData.macros[i] = data.macros[i];
            }
        }
        ShaderOption(const ShaderOption &src)
            : ReaderOption(src)
        {   
            optionData.filename = src.optionData.filename;
            optionData.type = src.optionData.type;
            optionData.numMacros = src.optionData.numMacros;
            optionData.macros = new char*[optionData.numMacros];
            for(uint i = 0; i < optionData.numMacros; i++)
            {
                optionData.macros[i] = src.optionData.macros[i];
            }
        }

        const ShaderOption &operator=(const ShaderOption &src)
        {
            _cacheHint = src._cacheHint;

            optionData.filename = src.optionData.filename;
            optionData.type = src.optionData.type;
            optionData.numMacros = src.optionData.numMacros;
            optionData.macros = new char*[optionData.numMacros];
            for(uint i = 0; i < optionData.numMacros; i++)
            {
                optionData.macros[i] = src.optionData.macros[i];
            }

            return *this;
        }


        virtual ~ShaderOption()
        {
            SAFE_DEL(optionData.macros);
        }

        OptionData optionData; 

        friend class ShaderReader;
        friend class ShaderManager;
    };

    typedef TextSource ShaderSource;
    typedef TextSource ShaderIncludeSource;

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

        void SetRenderResource(RenderResourceHandle &res) { _res = res; }
        const RenderResourceHandle &GetRenderResource() { return _res; }
        bool Compiled() { return _compiled; }

        ShaderSource::Ptr GetShaderSource() { return _source; }
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
        RenderResourceHandle _res;

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
        void SetRenderResource(RenderResourceHandle &res) { _res = res; }
        const RenderResourceHandle &GetRenderResource() { return _res; }

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
        RenderResourceHandle _res;
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
                const RenderResourceHandle &res = program->GetRenderResource();
                glUseProgram(res.id);
            }
        }

        ~ShaderProgramUse()
        {
            glUseProgram(0);
        }
    };


    class ShaderReader : public ResourceReader
    {
    public:
        ShaderReader();
        virtual ~ShaderReader();

        template <typename T>
        ReadResult<T> Read(const char *filename, ReaderOption *option);
        ReadResult<Shader> ReadAsync(const char *filename, ReaderOption *option);

        void OnSuccess(Object::Ptr obj)
        {
            Shader* shader = dynamic_cast<Shader*>(obj.get());
            if(shader != nullptr)
            {
                shader->SetupCompile();
                shader->Compile();

                Console::LogInfo("*****************************\n");
            }
        }

        void OnFailed()
        {}

        DECLARE_READERID;
    };

    class ShaderManager : public IUpdatable
    {
    public:
        ShaderManager();
        ~ShaderManager();

        virtual void Update() override;

        Shader::Ptr ReadShader(const char* filename, ShaderOption* option);
        ShaderProgram::Ptr ReadShaders(ShaderOption options[], int32 num);

        void ReadShaderAsync(const char* filename, ShaderOption* option);
        void ReadShadersAsync(ShaderOption options[], int32 num);

    private:
        std::vector<std::future<ReadResult<Shader>>> _futures;
        std::vector<Shader::Ptr> _shaders;
        std::mutex _mutex;
    };

} // namespace TwinkleGraphics

#endif