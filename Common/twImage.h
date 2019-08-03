

#ifndef TW_IMAGE_H
#define TW_IMAGE_H

#include "FreeImage.h"

#include "twCommon.h"

#include "twResource.hpp"

namespace TwinkleGraphics
{
class Image;
class ImageReader;
class ImageManager;
typedef Singleton<Image> ImageManagerInst;

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
    ImageReadInfo _info;
};


class Image : Object
{
public:
    typedef std::shared_ptr<Image> Ptr;

    Image();
    virtual ~Image();

    void SetHeight(int32 height) {}
    void SetWidth(int32 width) {}

private:
    int32 _height;
    int32 _width;

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