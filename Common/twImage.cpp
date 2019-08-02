
#include "twImage.h"

namespace TwinkleGraphics
{
template <>
ReadResult<Image::Ptr> ImageReader::Read<Image::Ptr>(const char *filename, ReaderOption *option)
{
    return ReadResult<Image::Ptr>();
}






Image::Image()
{}

Image::~Image()
{}






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

Image::Ptr ImageManager::ReadImage(ImageReadInfo &shader_info)
{
    Image::Ptr image = std::make_shared<Image>();
}

} // namespace TwinkleGraphics
