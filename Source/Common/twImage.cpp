#include "FreeImage.h"

#include "twImage.h"
#include "twTexture.h"
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
		: ResourceReader()
	{
		// INITIALISE_READERID
	}

    ImageReader::ImageReader(ImageOption* option)
        : ResourceReader()
    {
        if(option != nullptr)
        {
            if(_option != nullptr)
            {
                SAFE_DEL(_option); 
            }

			_option = new ImageOption(*option);
			_option->AddSuccessFunc(0, this, &ImageReader::OnReadImageSuccess);
			_option->AddFailedFunc(0, this, &ImageReader::OnReadImageFailed);
        }
    }

	ImageReader::~ImageReader()
	{
	}
	DEFINE_READERID(ImageReader)


	ReadResult<Image> ImageReader::Read(const char *filename)
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
			return ReadDDS(filename);
		}
		else
		{
			return ReadOthers(filename);
		}
	}

    ReadResult<Image> ImageReader::ReadAsync(std::string filename)
	{
        _asynchronize = true;
		return Read(filename.c_str());
	}

	ReadResult<Image> ImageReader::ReadDDS(const char *filename)
	{
		vglImageData data;
		if (vglLoadDDS(filename, &data))
		{
			Console::LogInfo("Image: Load image ", filename, " successed.\n");
			Image::Ptr image = std::make_shared<Image>(filename, data);
			return ReadResult<Image>(shared_from_this(), image, ReadResult<Image>::Status::SUCCESS);
		}

		Console::LogWarning("Image: Load image ", filename, " failed.\n");

		return ReadResult<Image>(ReadResult<Image>::Status::FAILED);
	}

	ReadResult<Image> ImageReader::ReadOthers(const char *filename)
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
		return ReadResult<Image>(shared_from_this(), ret_image, ReadResult<Image>::Status::SUCCESS);
	}

    void ImageReader::OnReadImageSuccess(Object::Ptr obj)
    {
        Image::Ptr image = std::dynamic_pointer_cast<Image>(obj);
        if (image != nullptr)
        {
			if(_option != nullptr)
			{
				ImageOption* op = dynamic_cast<ImageOption*>(_option);
				if(op != nullptr)
				{
				}
			}
        }
    }

    void ImageReader::OnReadImageFailed() 
    {}	

} // namespace TwinkleGraphics
