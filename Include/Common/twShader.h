
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
           
        ShaderOption(const OptionData& data);
        ShaderOption(const ShaderOption &src);
        const ShaderOption &operator=(const ShaderOption &src) = delete;
        virtual ~ShaderOption();

    private:
        OptionData _optionData;

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
        ShaderReader(ReaderOption* option);
        virtual ~ShaderReader();

        ReadResult<Shader> Read(const char *filename);
        ReadResult<Shader> ReadAsync(std::string filename);

        DECLARE_READERID;

    private:
    };

} // namespace TwinkleGraphics

#endif