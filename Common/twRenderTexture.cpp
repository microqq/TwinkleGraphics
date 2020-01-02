

#include "twRenderTexture.h"

namespace TwinkleGraphics
{
RenderTexture::RenderTexture(int32 width, int32 height, GLenum internalformat, GLenum format
        , bool usedepth, bool depthwithstencil
        , bool antialiasing, int32 samples
        , bool fixedsampledlocation)
    : Object()
    , _width(width)
    , _height(height)
    , _samples(samples)
    , _usedepth(usedepth)
    , _depthwithstencil(depthwithstencil)
    , _antialiasing(antialiasing)
    , _fixedsampledlocation(fixedsampledlocation)
{
    Create();
}

RenderTexture::~RenderTexture()
{}

void RenderTexture::Create()
{
    if(!_antialiasing)
    {
        _texture = std::make_shared<Texture2D>();
        _texture->Create(_width, _height, _internalformat, _format);
    }
    else
    {
        _texture = std::make_shared<Texture2DMultiSample>(_samples);
        _texture->Create(_width, _height, _internalformat, _format);
    }
    if(_usedepth)
    {
        if(_depthwithstencil)
        {

        }
    }
}

void RenderTexture::AttachToFramebuffer(FrameBufferObject::Ptr framebuf
        , AttachmentType type
        ,  bool genframebuf)
{

}

} // namespace TwinkleGraphics
