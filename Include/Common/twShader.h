
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
#include "twTextManager.h"

namespace TwinkleGraphics
{
    class Shader;
    class ShaderProgram;
    class ShaderResource;
    class ShaderOption;
    class ShaderReader;
    class ShaderManager;

    enum class ShaderType
    {
        VERTEX_SHADER = GL_VERTEX_SHADER,
        TCS_SHADER = GL_TESS_CONTROL_SHADER,
        TES_SHADER = GL_TESS_EVALUATION_SHADER,
        GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
        FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
        COMPUTE_SHADER = GL_COMPUTE_SHADER
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

        ShaderProgram(int32 shaderCount);
        virtual ~ShaderProgram();

        void AddShader(Shader::Ptr shader);
        void ClearShader();
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
        int32 _linkShaderCount = 0;
        GLint _linked = false;

        friend class ShaderManager;
    };

    class __TWCOMExport ShaderOption final : public ReaderOption
    {
    public:
        struct OptionData
        {
            std::string filename;
            ShaderType type;
            int32 numMacros = 0;
            char **macros = nullptr;
        };

        ShaderOption();
        ShaderOption(const OptionData& data);
        ShaderOption(const ShaderOption &src);
        const ShaderOption &operator=(const ShaderOption &src);
        virtual ~ShaderOption();

    private:
        OptionData _optionData;

        friend class ShaderReader;
        friend class ShaderManager;
    };

    class __TWCOMExport ShaderProgramOption final : public ReaderOption
    {
    public:
        ShaderProgramOption(ShaderOption* options, int num)
            : ReaderOption()
        {
            FillShaderOptions(options, num);
        }

        ShaderProgramOption(const ShaderProgramOption &src)
            : ReaderOption(src)
        {
            FillShaderOptions(src._shaderOptions, src._numShaderOption);
        }

        const ShaderProgramOption &operator=(const ShaderProgramOption &src) = delete;

        virtual ~ShaderProgramOption() 
        {
            SAFE_DEL_ARR(_shaderOptions);
        }
    private:
        void FillShaderOptions(ShaderOption* options, int num)
        {
            if(options == nullptr)
                return;
            
            SAFE_DEL_ARR(_shaderOptions);

            _shaderOptions = new ShaderOption[num];
            _numShaderOption = num;

            for(int i = 0; i < num; i++)
            {
                _shaderOptions[i] = options[i];
            }
        }


    private:
        ShaderOption* _shaderOptions = nullptr;
        int _numShaderOption;

        friend class ShaderReader;
        friend class ShaderManager;
    };

    class __TWCOMExport ShaderProgramUse
    {
    public:
        ShaderProgramUse(ShaderProgram::Ptr program);
        ~ShaderProgramUse();
    };

    class __TWCOMExport ShaderReader : public ResourceReader
        , public Reference<ShaderReader>
        , public INonCopyable
    {
    public:
        typedef std::shared_ptr<ShaderReader> Ptr;

        ShaderReader();
        ShaderReader(ShaderOption* option);
        ShaderReader(ShaderProgramOption* option);
        virtual ~ShaderReader();

        ReadResult<Shader> Read(const char *filename);
        ReadResult<Shader> ReadAsync(std::string filename);
        ReadResult<ShaderProgram> ReadProgramAsync(std::string filename);

        void SetOption(ShaderOption* option)
        {
            if(option == nullptr)
                return;

            if(_option != nullptr)
            {
                SAFE_DEL(_option);
            }
            _option = new ShaderOption(*option);
        }

        void SetOption(ShaderProgramOption* option)
        {
            if(option == nullptr)
                return;

            if(_option != nullptr)
            {
                SAFE_DEL(_option);
            }
            _option = new ShaderProgramOption(*option);
        }

        DECLARE_READERID;

    private:
    };

} // namespace TwinkleGraphics

#endif