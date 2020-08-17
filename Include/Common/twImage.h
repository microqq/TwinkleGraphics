

#ifndef TW_IMAGE_H
#define TW_IMAGE_H

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

class ImageResource : public Resource
{
public:
    ImageResource()
        : Resource()
    {}
    virtual ~ImageResource()
    {}

private:

};

class Image : public Object
{
public:
    typedef std::shared_ptr<Image> Ptr;

    Image();
    Image(const char* filename, const ImageData& data);
    virtual ~Image();

    void SetImageSource(ImageData &&data) { _data = data; }
    const ImageData& GetImageSource() { return _data; }
    ImageData* GetImageSourcePtr() { return &_data; }

    void SetFilename(const char* filename) { _filename = filename; }
    const std::string& GetFilename() { return _filename; }

private:
    ImageData _data;
    std::string _filename;
};

struct ImageReadInfo
{
    std::string filename;
};

class ImageReader
{
public:
    ImageReader(ImageReadInfo& info);
    ~ImageReader();

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