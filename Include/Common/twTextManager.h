#ifndef TW_TEXTMANAGER_H
#define TW_TEXTMANAGER_H

#include "twTextReader.h"

namespace TwinkleGraphics {
class __TWAPI TextManager : public IUpdatable,
                                  public INonCopyable,
                                  public IDestroyable {
public:
  virtual ~TextManager() { Destroy(); }
  TextSource::Ptr ReadText(const char *filename);
  ReadResult<TextSource> ReadTextAsync(const char *filename);

  virtual void Update(float deltaTime = 0.0f) override {}
  virtual void Destroy() override {
    {
      std::lock_guard<std::mutex> lock(_mutex);
      _futures.clear();
    }
  }

  void AddTaskFuture(std::future<ReadResult<TextSource>> future);

private:
  explicit TextManager() : IUpdatable(), INonCopyable(), IDestroyable() {}
  void OnReadTextSuccess(ObjectPtr obj);
  void OnReadTextFailed();

private:
  std::vector<std::future<ReadResult<TextSource>>> _futures;
  std::vector<TextSource::Ptr> _texts;
  std::mutex _mutex;

  friend class Singleton<TextManager>;
};

__TWAPI TextManager &TextMgrInstance();
} // namespace TwinkleGraphics

#endif