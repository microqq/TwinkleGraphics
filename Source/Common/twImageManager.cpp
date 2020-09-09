#include "twImageManager.h"
#include "twResourceManager.h"

namespace TwinkleGraphics
{
    ImageManager& ImageMgrInstance() { return Singleton<ImageManager>::Instance(); }

	ImageManager::ImageManager()
        : IUpdatable()
        , INonCopyable()
	{
// call this ONLY when linking with FreeImage as a static library
#ifdef FREEIMAGE_LIB
		FreeImage_Initialise();
#endif
	}

	ImageManager::~ImageManager()
	{
// call this ONLY when linking with FreeImage as a static library
#ifdef FREEIMAGE_LIB
		FreeImage_DeInitialise();
#endif
		Destroy();
	}

	void ImageManager::Destroy()
	{
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _futures.clear();
        }
	}

	Image::Ptr ImageManager::ReadImage(const char* filename)
	{
        ResourceManager& resMgr = ResourceMgrInstance();
		ReadResult<Image> result = resMgr.Read<ImageReader, Image>(filename, nullptr);
		Image::Ptr image = result.GetSharedObject();

		return image;
	}

    void ImageManager::ReadImageAsync(const char* filename)
	{
        ResourceManager& resMgr = ResourceMgrInstance();
		resMgr.ReadAsync<ImageReader, Image>(filename, nullptr);
	}    

    void ImageManager::AddTaskFuture(std::future<ReadResult<Image>> future)
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _futures.emplace_back(std::move(future));
        }
    }    

    template <>
    void ResourceManager::PackedReadTask<ReadResult<Image>, ImageReader>::PushTask()
    {
        // typedef Ret(R::*)(const char*, ReaderOption) Func;
        ResourceManager &resMgr = ResourceMgrInstance();
        auto future = resMgr.PushTask(&ImageReader::ReadAsync, _reader, _filename);
		{
			ImageManager& imgMgr = ImageMgrInstance();
			imgMgr.AddTaskFuture(std::move(future));
		}
    }	
}