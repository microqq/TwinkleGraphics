
#ifndef TW_SHADERMANAGER_H
#define TW_SHADERMANAGER_H

#include "twShader.h"
#include "twSingleton.h"

namespace TwinkleGraphics
{
    class __TWCOMExport ShaderManager : public IUpdatable
        , public INonCopyable
        , public IDestroyable
    {
    public:
        virtual ~ShaderManager();

        /**
         * @brief 
         * Update() must execute in main thread
         */
        virtual void Update() override;
        virtual void Destroy() override;

        Shader::Ptr ReadShader(const char* filename, ShaderOption* option);
        ShaderProgram::Ptr ReadShaders(ShaderOption options[], int32 num);

        ReadResult<Shader> ReadShaderAsync(const char* filename, ShaderOption* option);
        ShaderProgram::Ptr ReadShadersAsync(ShaderOption options[], int32 num);
        void AddTaskFuture(std::future<ReadResult<Shader>> future);

        void OnReadShaderSuccess(Object::Ptr obj);
        void OnReadShaderFailed();

    private:
        explicit ShaderManager();

    private:
        std::vector<std::future<ReadResult<Shader>>> _futures;
        std::mutex _mutex;

        friend class Singleton<ShaderManager>;
    };

#ifdef __cplusplus
    extern "C"
    {
#endif
        __TWCOMExport ShaderManager& ShaderMgrInstance();
#ifdef __cplusplus
    }
#endif

    typedef Singleton<ShaderManager> ShaderManagerInst;

} // namespace TwinkleGraphics

#endif