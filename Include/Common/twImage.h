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
class ImageManager;
typedef Singleton<ImageManager> ImageManagerInst;

struct ImageSource
{
    typedef std::shared_ptr<ImageSource> Ptr;
    typedef std::weak_ptr<ImageSource> WeakPtr;

    ImageData imagedata;
    std::string filename;
};

class Image : public Object
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
    ImageSource::Ptr _source;
};

struct ImageReadInfo
{
    std::string filename;
};

class ImageReader final : public ResourceReader
{
public:
    ImageReader(ImageReadInfo& info);
    virtual ~ImageReader();

    template <typename TPtr>
    ReadResult<TPtr> Read(const char *filename, ReaderOption *option);

private:
    ReadResult<Image::Ptr> ReadDDS(const char *filename, ReaderOption *option);
    ReadResult<Image::Ptr> ReadOthers(const char *filename, ReaderOption *option);

private:
    DECLARE_READERID;

    ImageReadInfo _info;
};

class ImageManager
{
public:
    ImageManager();
    ~ImageManager();

    void ReadImages(ImageReadInfo images_info[], Image::Ptr images[], int num);
    Image::Ptr ReadImage(ImageReadInfo& image_info);

private:
    
};

} // namespace TwinkleGraphics


#endif