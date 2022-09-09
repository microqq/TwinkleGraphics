
#ifndef TW_SHADERMANAGER_H
#define TW_SHADERMANAGER_H

#include "twReaderManager.h"
#include "twShader.h"
#include "twSingleton.h"

namespace TwinkleGraphics {
class __TWCOMExport ShaderManager : public IUpdatable,
                                    public IReaderManager,
                                    public INonCopyable,
                                    public IDestroyable {
public:
  virtual ~ShaderManager();

  /**
   * @brief
   * Update() must execute in main thread
   */
  virtual void Update(float deltaTime = 0.0f) override;
  virtual void Destroy() override;

  ShaderPtr ReadShader(const char *filename, ShaderOption *option);
  ShaderProgramPtr ReadShaders(ShaderOption options[], int32 num);

  ReadResult<Shader> ReadShaderAsync(const char *filename,
                                     ShaderOption *option);
  ReadResult<ShaderProgram> ReadShadersAsync(ShaderProgramOption *option,
                                             int32 num);
  void AddTaskFuture(std::future<ReadResult<Shader>> future);
  void AddTaskProgramFuture(std::future<ReadResult<ShaderProgram>> future);

private:
  using ShaderFutures = std::vector<std::future<ReadResult<Shader>>>;
  using ShaderProgramFutures =
      std::vector<std::future<ReadResult<ShaderProgram>>>;

  explicit ShaderManager();
  void OnReadShaderSuccess(ObjectPtr obj);
  void OnReadShaderFailed();

  void OnReadShadersSuccess(ObjectPtr obj);
  void OnReadShadersFailed();

private:
  std::vector<std::future<ReadResult<Shader>>> _futures;
  std::vector<std::future<ReadResult<ShaderProgram>>> _programFutures;
  std::mutex _mutex;
  std::mutex _programMutex;

  friend class Singleton<ShaderManager>;
};

__TWCOMExport ShaderManager &ShaderMgrInstance();
} // namespace TwinkleGraphics

#endif