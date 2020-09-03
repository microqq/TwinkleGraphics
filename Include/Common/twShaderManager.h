
#ifndef TW_SHADERMANAGER_H
#define TW_SHADERMANAGER_H

#include "twShader.h"
#include "twSingleton.h"

namespace TwinkleGraphics
{
    class __SINGLETONExport ShaderManager : public IUpdatable, public INonCopyable
    {
    public:
        virtual ~ShaderManager();
        virtual void Update() override;

        Shader::Ptr ReadShader(const char* filename, ShaderOption* option);
        ShaderProgram::Ptr ReadShaders(ShaderOption options[], int32 num);

        void ReadShaderAsync(const char* filename, ShaderOption* option);
        void ReadShadersAsync(ShaderOption options[], int32 num);

    private:
        explicit ShaderManager();

    private:
        std::vector<std::future<ReadResult<Shader>>> _futures;
        std::vector<Shader::Ptr> _shaders;
        std::mutex _mutex;

        friend class Singleton<ShaderManager>;
    };

#ifdef __cplusplus
    extern "C"
    {
#endif
        __SINGLETONExport ShaderManager& ShaderMgrInstance();
#ifdef __cplusplus
    }
#endif

    typedef Singleton<ShaderManager> ShaderManagerInst;

} // namespace TwinkleGraphics

#endif