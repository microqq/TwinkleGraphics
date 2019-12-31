

#include "twRenderTexture.h"

namespace TwinkleGraphics
{
RenderTexture::RenderTexture(int32 width, int32 height, GLenum internalformat, GLenum format
        , bool antialiasing, int32 samples, bool hasdepthstencil, int32 depth, bool fixedsampledlocation)
    : Object()
{
    InitStorage(width, height, internalformat, format, antialiasing, samples
        , hasdepthstencil, depth, fixedsampledlocation);
}

RenderTexture::~RenderTexture()
{

}

void RenderTexture::InitStorage(int32 width, int32 height, GLenum internalformat, GLenum format
        , bool antialiasing, int32 samples, bool hasdepthstencil, int32 depth, bool fixedsampledlocation)
{
    if(!antialiasing)
    {
        _texture = std::make_shared<Texture2D>();
        _texture->Create(width, height, internalformat, format);
    }
    else
    {
        if(depth > 1)
        {
            _texture = std::make_shared<Texture2DMultiSampleArray>(samples);
            _texture->Create(width, height, internalformat, format, 1, depth, -1);
        }
        else
        {
            _texture = std::make_shared<Texture2DMultiSample>(samples);
            _texture->Create(width, height, internalformat, format, 1, depth, -1);
        }
    }    
}

} // namespace TwinkleGraphics
