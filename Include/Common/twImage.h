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

struct ImageSource : public SourceHandle
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

class ImageReader final : public ResourceReader
{
public:
    ImageReader();
    virtual ~ImageReader();

    template <typename TPtr>
    ReadResult<TPtr> Read(const char *filename, ReaderOption *option);

    template <typename TPtr>
    ReadResult<TPtr> ReadAsync(const char *filename, ReaderOption *option);

    // virtual void ReadAsync(const char *filename, ReaderOption *option) override;

private:
    ReadResult<Image::Ptr> ReadDDS(const char *filename, ReaderOption *option);
    ReadResult<Image::Ptr> ReadOthers(const char *filename, ReaderOption *option);

    DECLARE_READERID;
};

class ImageManager
{
public:
    ImageManager();
    ~ImageManager();

    Image::Ptr ReadImage(const char* filename);

private:
    
};

} // namespace TwinkleGraphics


#endif