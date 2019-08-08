

#ifndef TW_IMAGE_H
#define TW_IMAGE_H

#include <vector>
#include "vermilion.h"

#include "twCommon.h"

#include "twResource.hpp"

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

class Image : public Object
{
public:
    typedef std::shared_ptr<Image> Ptr;

    Image();
    virtual ~Image();

    void SetData(ImageData &data) { _data = data; }
    const ImageData& GetSubImageData() { return _data; }

private:
    ImageData _data;
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
    
    Image::Ptr ReadImage(ImageReadInfo& shader_info);

private:
    
};

} // namespace TwinkleGraphics


#endif