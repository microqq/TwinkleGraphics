

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
typedef Singleton<ImageManager> ImageManagerInst;

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
    Image(BYTE* bytes, int32 width, int32 height, FREE_IMAGE_FORMAT fif = FREE_IMAGE_FORMAT::FIF_UNKNOWN);
    virtual ~Image();

    void SetWidth(int32 width) { _width = width; }
    void SetHeight(int32 height) { _height = height; }
    void SetRawData(BYTE* data) { _raw_data = data; }

    int32 GetWidth() { return _width; }
    int32 GetHeight() { return _height; }
    BYTE* GetRawDatqa() { return _raw_data; }
    FREE_IMAGE_FORMAT GetFormart() { return _fif; }

private:
    BYTE* _raw_data;

    FREE_IMAGE_FORMAT _fif;
    int32 _width;
    int32 _height;
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