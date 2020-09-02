#include "FreeImage.h"

#include "twImage.h"
#include "twConsoleLog.h"

#ifdef __cplusplus
extern "C"
{
#endif
	bool vglLoadDDS(const char *filename, vglImageData *image);
#ifdef __cplusplus
}
#endif

namespace TwinkleGraphics
{
	Image::Image()
		: Object()
	{
		_source = std::make_shared<ImageSource>();
	}

	Image::Image(const char *filename, const ImageData &data)
		: Object()
	{
		_source = std::make_shared<ImageSource>();
		_source->imagedata = data;
		_source->filename = filename;
	}
	Image::~Image()
	{
	}

	ImageReader::ImageReader()
	{
		// INITIALISE_READERID
	}
	ImageReader::~ImageReader()
	{
	}
	DEFINE_READERID(ImageReader)


	template <>
	ReadResult<Image> ImageReader::Read<Image>(const char *filename, ReaderOption *option)
	{
		//image format
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		//check the file signature and deduce its format
		fif = FreeImage_GetFileType(filename, 0);
		//if still unknown, try to guess the file format from the file extension
		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(filename);
		//if still unkown, return failure
		if (fif == FIF_UNKNOWN)
			return ReadResult<Image>(ReadResult<Image>::Status::FAILED);

		if (fif == FIF_DDS)
		{
			return ReadDDS(filename, option);
		}
		else
		{
			return ReadOthers(filename, option);
		}
	}

    ReadResult<Image> ImageReader::ReadAsync(const char *filename, ReaderOption *option)
	{
		return Read<Image>(filename, option);
	}

	ReadResult<Image> ImageReader::ReadDDS(const char *filename, ReaderOption *option)
	{
		vglImageData data;
		if (vglLoadDDS(filename, &data))
		{
			Console::LogInfo("Image: Load image ", filename, " successed.\n");
			Image::Ptr image = std::make_shared<Image>(filename, data);
			return ReadResult<Image>(image, ReadResult<Image>::Status::SUCCESS);
		}

		Console::LogWarning("Image: Load image ", filename, " failed.\n");

		return ReadResult<Image>(ReadResult<Image>::Status::FAILED);
	}

	ReadResult<Image> ImageReader::ReadOthers(const char *filename, ReaderOption *option)
	{
		//image format
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		//pointer to the image, once loaded
		FIBITMAP *dib(0);
		//pointer to the image data
		BYTE *bits(0);
		//image width and height
		unsigned int width(0), height(0);

		//check the file signature and deduce its format
		fif = FreeImage_GetFileType(filename, 0);
		//if still unknown, try to guess the file format from the file extension
		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(filename);
		//if still unkown, return failure
		if (fif == FIF_UNKNOWN)
			return ReadResult<Image>(ReadResult<Image>::Status::FAILED);

		//check that the plugin has reading capabilities and load the file
		if (FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, filename);
		//if the image failed to load, return failure
		if (!dib)
		{
			Console::LogWarning("Image: Load image ", filename, " failed.\n");
			return ReadResult<Image>(ReadResult<Image>::Status::FAILED);
		}

		FIBITMAP *image = FreeImage_ConvertTo32Bits(dib);

		//retrieve the image data
		bits = FreeImage_GetBits(image);
		//get the image width and height
		width = FreeImage_GetWidth(image);
		height = FreeImage_GetHeight(image);

		//if this somehow one of these failed (they shouldn't), return failure
		if ((bits == 0) || (width == 0) || (height == 0))
		{
			Console::LogWarning("Image: Load image ", filename, " failed(...).\n");
			return ReadResult<Image>(ReadResult<Image>::Status::FAILED);
		}

		ImageData data;
		data.target = GL_TEXTURE_2D;
		data.internalFormat = GL_RGBA8;
		data.format = GL_BGRA;
		data.type = GL_UNSIGNED_BYTE;
		data.mipLevels = 1;
		data.totalDataSize = width * height * 4;

		data.mip[0].width = width;
		data.mip[0].height = height;
		data.mip[0].mipStride = data.totalDataSize;
		data.mip[0].data = bits;

		Image::Ptr ret_image = std::make_shared<Image>(filename, data);

		//Free FreeImage's copy of the data
		FreeImage_Unload(dib);

		Console::LogInfo("Image: Load image ", filename, " successed.\n");

		//return success
		return ReadResult<Image>(ret_image, ReadResult<Image>::Status::SUCCESS);
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

	Image::Ptr ImageManager::ReadImage(const char* filename)
	{
        ResourceManager& resMgr = ResourceManagerInst::Instance();
		ReadResult<Image> result = resMgr.Read<ImageReader, Image>(filename, nullptr);
		Image::Ptr image = result.GetSharedObject();

		return image;
	}

    auto ImageManager::ReadImageAsync(const char* filename)
		-> std::future<ReadResult<Image>>
	{
        ResourceManager& resMgr = ResourceManagerInst::Instance();
		return resMgr.ReadAsync<ImageReader, Image>(filename, nullptr);
	}
} // namespace TwinkleGraphics
