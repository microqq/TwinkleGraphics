#ifndef TW_IMAGEMANAGER_H
#define TW_IMAGEMANAGER_H

#include "twImage.h"

namespace TwinkleGraphics
{
    class __TWCOMExport ImageManager : public IUpdatable, public INonCopyable, public IDestroyable
    {
    public:
        virtual ~ImageManager();
        virtual void Update() override {}
        virtual void Destroy() override;

        Image::Ptr ReadImage(const char *filename);
        ReadResult<Image> ReadImageAsync(const char *filename);

        void AddTaskFuture(std::future<ReadResult<Image>> future);

    private:
        explicit ImageManager();
        void OnReadShaderSuccess(Object::Ptr obj);
        void OnReadShaderFailed();

    private:
        std::vector<std::future<ReadResult<Image>>> _futures;
        std::vector<Image::Ptr> _images;
        std::mutex _mutex;

        friend class Singleton<ImageManager>;
    };

#ifdef __cplusplus
    extern "C"
    {
#endif
        __TWCOMExport ImageManager &ImageMgrInstance();
#ifdef __cplusplus
    }
#endif

    typedef Singleton<ImageManager> ImageManagerInst;

} // namespace TwinkleGraphics

#endif