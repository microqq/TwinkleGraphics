#ifndef TW_TEXTUREMANAGER_H
#define TW_TEXTUREMANAGER_H

#include "twCommon.h"
#include "twTexture.h"
#include "twReaderManager.h"

namespace TwinkleGraphics
{
    class TextureReader;
    class TextureManager;

    class TextureOption final : public ReaderOption
    {
    public:
        TextureOption()
            : ReaderOption()
            , _texture(nullptr)
        {}
        TextureOption(const TextureOption &src)
            : ReaderOption(src)
        {
            _texture = src._texture;
        }
        const TextureOption &operator=(const TextureOption &src) = delete;
        virtual ~TextureOption() 
        {
            _texture = nullptr;
        }

        void SetImmutable(bool immutable) { _immutable = immutable; }
        void SetGenMipMap(bool genMipMap) { _genMipMap = genMipMap; }

    private:
        Texture::Ptr _texture = nullptr;
        bool _immutable = true;
        bool _genMipMap = false;

        friend class TextureReader;
        friend class TextureManager;
    };

    class TextureReader : public ResourceReader
        , public Reference<TextureReader>
        , public INonCopyable
    {
    public:
        typedef std::shared_ptr<TextureReader> Ptr;

        TextureReader();
        TextureReader(ReaderOption *option);
        virtual ~TextureReader();

        ReadResult<Texture> Read(const char *filename);
        ReadResult<Texture> ReadAsync(std::string filename);

        DECLARE_READERID;
    };

    class __TWCOMExport TextureManager : public IUpdatable
        , public IReaderManager
        , public INonCopyable
        , public IDestroyable
    {
    public:
        virtual ~TextureManager() 
        {
            Destroy();
        }
        Texture::Ptr ReadTexture(const char* filename, TextureOption* option);
        ReadResult<Texture> ReadTextureAsync(const char* filename, TextureOption* option);

        virtual void Update() override {}
        virtual void Destroy() override
        {
            {
                std::lock_guard<std::mutex> lock(_mutex);
                _futures.clear();
            }
        }

        void AddTaskFuture(std::future<ReadResult<Texture>> future);
    private:
        explicit TextureManager()
            : IUpdatable()
            , INonCopyable()            
        {}
        void OnReadTextureSuccess(Object::Ptr obj);
        void OnReadTextureFailed();

    private:
        std::vector<std::future<ReadResult<Texture>>> _futures;
        std::mutex _mutex;

        friend class Singleton<TextureManager>;
    };

#ifdef __cplusplus
    extern "C"
    {
#endif
        __TWCOMExport TextureManager& TextureMgrInstance();
#ifdef __cplusplus
    }
#endif    

    typedef Singleton<TextureManager> TextManagerInst;
} // namespace TwinkleGraphics


#endif