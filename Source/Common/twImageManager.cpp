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

    void ImageManager::Update(float deltaTime)
	{
		{
			std::lock_guard<std::mutex> lock(_mutex);
			RemoveFutures<Image>(_futures);
		}
	}

	void ImageManager::Destroy()
	{
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _futures.clear();
        }
	}

	Image::Ptr ImageManager::ReadImage(const char* filename, ImageOption* option)
	{
        ResourceManager& resMgr = ResourceMgrInstance();
		ReadResult<Image> result = resMgr.Read<ImageReader, Image, ImageOption>(filename, option);
		Image::Ptr image = result.GetSharedObject();

		return image;
	}

    ReadResult<Image> ImageManager::ReadImageAsync(const char* filename, ImageOption* option)
	{
        ResourceManager& resMgr = ResourceMgrInstance();
		return resMgr.ReadAsync<ImageReader, Image, ImageOption>(filename, option);
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
		if(_asyncRead)
		{
			auto future = resMgr.PushAsyncTask(&ImageReader::ReadAsync, _reader, _filename);
			{
				ImageManager& imgMgr = ImageMgrInstance();
				imgMgr.AddTaskFuture(std::move(future));
			}
		}
		else
		{
			resMgr.PushTask(&ImageReader::Read, _reader, _filename.c_str());
		}
    }	
}