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
    ImageSource::Ptr _source;
};

class __TWCOMExport ImageReader final : public ResourceReader, public Reference<ImageReader>, public INonCopyable
{
public:
    ImageReader();
    ImageReader(ReaderOption* option);
    virtual ~ImageReader();

    ReadResult<Image> Read(const char *filename);
    ReadResult<Image> ReadAsync(std::string filename);

private:
    ReadResult<Image> ReadDDS(const char *filename);
    ReadResult<Image> ReadOthers(const char *filename);

    DECLARE_READERID;
};

} // namespace TwinkleGraphics


#endif