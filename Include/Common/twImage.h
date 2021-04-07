#ifndef TW_IMAGE_H
#define TW_IMAGE_H

#include <memory>
#include <vector>

#include "vermilion.h"
#include "twCommon.h"
#include "twResource.h"

namespace TwinkleGraphics
{
typedef vglImageData ImageData;
typedef vglImageMipData SubImageData;


class Image;
class ImageReader;

struct ImageSource : public Object
{
    typedef std::shared_ptr<ImageSource> Ptr;
    typedef std::weak_ptr<ImageSource> WeakPtr;

    ImageData imagedata;
    std::string filename;
};

typedef ImageSource::Ptr ImageSourcePtr;

class __TWCOMExport Image : public Object
{
public:
    typedef std::shared_ptr<Image> Ptr;

    Image();
    Image(const char* filename, const ImageData& data);
    virtual ~Image();

    void SetImageSource(ImageData &&data) { _source->imagedata = data; }
    const ImageData& GetImageSource() { return _source->imagedata; }
    ImageData* GetImageSourcePtr() { return &_source->imagedata; }

    void SetFilename(const char* filename) { _source->filename = filename; }
    const std::string& GetFilename() { return _source->filename; }

private:
    ImageSourcePtr _source;
};

typedef Image::Ptr ImagePtr;

class ImageOption final : public ReaderOption
{
    public:
        ImageOption()
            : ReaderOption()
        {}
        ImageOption(const ImageOption &src)
            : ReaderOption(src)
        {
        }

        const ImageOption &operator=(const ImageOption &src) = delete;

        virtual ~ImageOption() 
        {
        }

    private:
        friend class ImageReader;
        friend class ImageManager;
};

class __TWCOMExport ImageReader final : public ResourceReader
        , public Reference<ImageReader>
        , public INonCopyable
{
public:
    typedef std::shared_ptr<ImageReader> Ptr;
    
    ImageReader();
    ImageReader(ImageOption* option);
    virtual ~ImageReader();

    ReadResult<Image> Read(const char *filename);
    ReadResult<Image> ReadAsync(std::string filename);

    void SetOption(ImageOption* option) 
    {
        if (option == nullptr)
            return;

        if (_option != nullptr)
        {
            SAFE_DEL(_option);
        }
        _option = new ImageOption(*option);
    }

private:
    ReadResult<Image> ReadDDS(const char *filename);
    ReadResult<Image> ReadOthers(const char *filename);

    void OnReadImageSuccess(ObjectPtr obj);
    void OnReadImageFailed();

    DECLARE_READERID;
};

} // namespace TwinkleGraphics


#endif