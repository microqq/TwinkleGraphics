
#ifndef TW_SHADERMANAGER_H
#define TW_SHADERMANAGER_H

#include "twShader.h"
#include "twSingleton.h"

namespace TwinkleGraphics
{
    class __SHADERExport ShaderManager : public IUpdatable
    {
    public:
        friend class Singleton<ShaderManager>;
        static ShaderManager& Instance() { return Singleton<ShaderManager>::Instance(); }

        ShaderManager(const ShaderManager&) = delete;
        ShaderManager(ShaderManager&&) = delete;
        ShaderManager& operator=(const ShaderManager&) = delete;
        ShaderManager& operator=(ShaderManager&&) = delete;

        virtual void Update() override;

        Shader::Ptr ReadShader(const char* filename, ShaderOption* option);
        ShaderProgram::Ptr ReadShaders(ShaderOption options[], int32 num);

        void ReadShaderAsync(const char* filename, ShaderOption* option);
        void ReadShadersAsync(ShaderOption options[], int32 num);

    private:
        explicit ShaderManager();
        ~ShaderManager();

    private:
        std::vector<std::future<ReadResult<Shader>>> _futures;
        std::vector<Shader::Ptr> _shaders;
        std::mutex _mutex;
    };

#ifdef __cplusplus
    extern "C"
    {
#endif
        __SHADERExport ShaderManager& ShaderMgrInstance();
#ifdef __cplusplus
    }
#endif

    typedef Singleton<ShaderManager> ShaderManagerInst;

} // namespace TwinkleGraphics

#endif