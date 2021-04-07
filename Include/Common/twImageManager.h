#ifndef TW_IMAGEMANAGER_H
#define TW_IMAGEMANAGER_H

#include "twImage.h"
#include "twReaderManager.h"

namespace TwinkleGraphics
{
    class __TWCOMExport ImageManager : public IUpdatable
        , public IReaderManager
        , public INonCopyable
        , public IDestroyable
    {
    public:
        virtual ~ImageManager();
        virtual void Update(float deltaTime = 0.0f) override;
        virtual void Destroy() override;

        ImagePtr ReadImage(const char *filename, ImageOption* option = nullptr);
        ReadResult<Image> ReadImageAsync(const char *filename, ImageOption* option = nullptr);

        void AddTaskFuture(std::future<ReadResult<Image>> future);

    private:
        explicit ImageManager();
    private:
        std::vector<std::future<ReadResult<Image>>> _futures;
        std::mutex _mutex;

        friend class Singleton<ImageManager>;
    };

    __TWCOMExport ImageManager &ImageMgrInstance();

} // namespace TwinkleGraphics

#endif