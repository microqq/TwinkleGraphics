#ifndef TW_TEXTUREMANAGER_H
#define TW_TEXTUREMANAGER_H

#include "twCommon.h"
#include "twReaderManager.h"
#include "twTexture.h"

namespace TwinkleGraphics {
class TextureReader;
class TextureManager;

class TextureOption final : public ReaderOption {
public:
  TextureOption(TextureType type) : ReaderOption(), _textureType(type) {}
  TextureOption(const TextureOption &src) : ReaderOption(src) {
    _immutable = src._immutable;
    _genMipMap = src._genMipMap;
    _textureType = src._textureType;
  }
  const TextureOption &operator=(const TextureOption &src) = delete;
  virtual ~TextureOption() {}

  void SetImmutable(bool immutable) { _immutable = immutable; }
  void SetGenMipMap(bool genMipMap) { _genMipMap = genMipMap; }

private:
  TextureType _textureType;
  bool _immutable = true;
  bool _genMipMap = false;

  friend class TextureReader;
  friend class TextureManager;
};

class TextureReader : public ResourceReader,
                      public Reference<TextureReader>,
                      public INonCopyable {
public:
  using Ptr = std::shared_ptr<TextureReader>;

  TextureReader();
  TextureReader(TextureOption *option);
  virtual ~TextureReader();

  ReadResult<Texture> Read(const char *filename);
  ReadResult<Texture> ReadAsync(std::string filename);

  void SetOption(TextureOption *option) {
    if (option == nullptr)
      return;

    if (_option != nullptr) {
      SAFE_DEL(_option);
    }
    _option = new TextureOption(*option);
  }

  DECLARE_READERID;
};

class __TWCOMExport TextureManager : public IUpdatable,
                                     public IReaderManager,
                                     public INonCopyable,
                                     public IDestroyable {
public:
  virtual ~TextureManager() { Destroy(); }
  TexturePtr ReadTexture(const char *filename, TextureOption *option);
  ReadResult<Texture> ReadTextureAsync(const char *filename,
                                       TextureOption *option);

  virtual void Update(float deltaTime = 0.0f) override {
    {
      std::lock_guard<std::mutex> lock(_mutex);
      RemoveFutures(_futures);
    }
  }
  virtual void Destroy() override {
    {
      std::lock_guard<std::mutex> lock(_mutex);
      _futures.clear();
    }
  }

  void AddTaskFuture(std::future<ReadResult<Texture>> future);

private:
  explicit TextureManager() : IUpdatable(), INonCopyable(), IDestroyable() {}
  void OnReadTextureSuccess(ObjectPtr obj);
  void OnReadTextureFailed();

private:
  std::vector<std::future<ReadResult<Texture>>> _futures;
  std::mutex _mutex;

  friend class Singleton<TextureManager>;
};

__TWCOMExport TextureManager &TextureMgrInstance();
} // namespace TwinkleGraphics

#endif