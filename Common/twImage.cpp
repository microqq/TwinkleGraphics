#include "FreeImage.h"

#include "twImage.h"


extern "C" bool vglLoadDDS(const char* filename, vglImageData* image);

namespace TwinkleGraphics
{

Image::Image(const char* filename, const ImageData& data)
	: Object()
	, _data(data)
	, _filename(filename)
{}
Image::~Image()
{}




ImageReader::ImageReader(ImageReadInfo &info)
	:_info(info)
{
}
ImageReader::~ImageReader()
{
}

template <>
ReadResult<Image::Ptr> ImageReader::Read<Image::Ptr>(const char *filename, ReaderOption *option)
{
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if(fif == FIF_UNKNOWN) 
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if(fif == FIF_UNKNOWN)
        return ReadResult<Image::Ptr>(ReadResult<Image::Ptr>::Status::FAILED);

	if(fif == FIF_DDS)
	{
		return ReadDDS(filename, option);
	}
	else
	{
		return ReadOthers(filename, option);
	}
}

ReadResult<Image::Ptr> ImageReader::ReadDDS(const char *filename, ReaderOption *option)
{
	vglImageData data;
	if(vglLoadDDS(filename, &data))
	{
		Image::Ptr image = std::make_shared<Image>(filename, data);		
		return ReadResult<Image::Ptr>(image, ReadResult<Image::Ptr>::Status::FAILED);
	}
        
	return ReadResult<Image::Ptr>(ReadResult<Image::Ptr>::Status::FAILED);
}

ReadResult<Image::Ptr> ImageReader::ReadOthers(const char *filename, ReaderOption *option)
{
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	
	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if(fif == FIF_UNKNOWN) 
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if(fif == FIF_UNKNOWN)
        return ReadResult<Image::Ptr>(ReadResult<Image::Ptr>::Status::FAILED);

	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if(!dib)
        return ReadResult<Image::Ptr>(ReadResult<Image::Ptr>::Status::FAILED);

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if((bits == 0) || (width == 0) || (height == 0))
        return ReadResult<Image::Ptr>(ReadResult<Image::Ptr>::Status::FAILED);
	
	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);

	ImageData data;
	Image::Ptr image = std::make_shared<Image>(filename, data);

	//return success
    return ReadResult<Image::Ptr>(image, ReadResult<Image::Ptr>::Status::SUCCESS);
}





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

Image::Ptr ImageManager::ReadImage(ImageReadInfo &image_info)
{
    ResourceManagerInst resMgr;
    ReadResult<Image::Ptr> result = resMgr->Read<ImageReader, Image::Ptr>(image_info.filename.c_str(), nullptr, image_info);
    Image::Ptr image = result.GetSharedObject();

    return image;
}

} // namespace TwinkleGraphics
