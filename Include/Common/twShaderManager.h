
#ifndef TW_SHADERMANAGER_H
#define TW_SHADERMANAGER_H

#include "twReaderManager.h"
#include "twShader.h"
#include "twSingleton.h"

namespace TwinkleGraphics
{
    class __TWCOMExport ShaderManager : public IUpdatable
        , public IReaderManager
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
        ReadResult<ShaderProgram> ReadShadersAsync(ShaderOption options[], int32 num);
        void AddTaskFuture(std::future<ReadResult<Shader>> future);
        void AddTaskProgramFuture(std::future<ReadResult<ShaderProgram>> future);

    private:
        typedef std::vector<std::future<ReadResult<Shader>>> ShaderFutures;
        typedef std::vector<std::future<ReadResult<ShaderProgram>>> ShaderProgramFutures;

        explicit ShaderManager();
        void OnReadShaderSuccess(Object::Ptr obj);
        void OnReadShaderFailed();

        void OnReadShadersSuccess(Object::Ptr obj, ShaderOption* option);
        void OnReadShadersFailed();

    private:
        std::vector<std::future<ReadResult<Shader>>> _futures;
        std::vector<std::future<ReadResult<ShaderProgram>>> _programFutures;
        std::mutex _mutex;
        std::mutex _programMutex;

        friend class Singleton<ShaderManager>;
    };

#ifdef __cplusplus
        __TWCOMExport ShaderManager& ShaderMgrInstance();
#endif

    typedef Singleton<ShaderManager> ShaderManagerInst;

} // namespace TwinkleGraphics

#endif