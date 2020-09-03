#include "twImageManager.h"
#include "twResourceManager.h"

namespace TwinkleGraphics
{
    ImageManager& ImageMgrInstance() { return Singleton<ImageManager>::Instance(); }

	ImageManager::ImageManager()
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
		auto future = resMgr.ReadAsync<ImageReader, Image>(filename, nullptr);
	}    
}