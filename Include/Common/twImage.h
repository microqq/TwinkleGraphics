

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


// class SubImage : public Object
// {
// public:
//     typedef std::shared_ptr<SubImage> Ptr;

//     SubImage()
//         : Object()
//     {}
//     virtual ~SubImage()
//     {
//     }

//     void SetData(SubImageData &data) { _data = data; }
//     const SubImageData& GetSubImageData() { return _data; }

// private:
//     SubImageData _data;
// };

struct ImageSource
{
    typedef std::shared_ptr<ImageSource> Ptr;
    typedef std::weak_ptr<ImageSource> WeakPtr;

    ImageData _data;
    std::string _filename;
};

class Image : public Object
{
public:
    typedef std::shared_ptr<Image> Ptr;

    Image();
    Image(const char* filename, const ImageData& data);
    virtual ~Image();

    void SetImageSource(ImageData &&data) { _source._data = data; }
    const ImageData& GetImageSource() { return _source._data; }
    ImageData* GetImageSourcePtr() { return &_source._data; }

    void SetFilename(const char* filename) { _source._filename = filename; }
    const std::string& GetFilename() { return _source._filename; }

private:
    ImageSource _source;
    // ImageData _data;
    // std::string _filename;
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