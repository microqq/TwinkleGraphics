

#include "twRenderTexture.h"

namespace TwinkleGraphics
{
RenderTexture::RenderTexture(int32 width, int32 height, GLenum internalformat, GLenum format
        , bool usedepth, bool depthwithstencil
        , bool antialiasing, int32 samples
        , bool fixedsampledlocation)
    : Object()
    , _texture(nullptr)
    , _depthbuf(nullptr)
    , _framebuf(nullptr)
    , _width(width)
    , _height(height)
    , _samples(samples)
    , _internalformat(internalformat)
    , _format(format)
    , _usedepth(usedepth)
    , _depthwithstencil(depthwithstencil)
    , _antialiasing(antialiasing)
    , _fixedsampledlocation(fixedsampledlocation)
{
}

RenderTexture::~RenderTexture()
{}

void RenderTexture::Create(bool autogenFBO)
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
            _depthbuf = std::make_shared<RenderBufferObject>(_width, _height, GL_DEPTH24_STENCIL8, _samples, _antialiasing);
        }
        else
        {
            _depthbuf = std::make_shared<RenderBufferObject>(_width, _height, GL_DEPTH_COMPONENT, _samples, _antialiasing);
        }
    }

    if(autogenFBO)
    {
        _framebuf = std::make_shared<FrameBufferObject>();
        switch (_format)
        {
        case GL_DEPTH_COMPONENT:
            AttachToFramebuffer(AttachmentType::DEPTH_ATTACHMENT);
            break;
        case GL_DEPTH_STENCIL:        
            AttachToFramebuffer(AttachmentType::DEPTH_STENCIL);
            break;
        case GL_STENCIL_INDEX:
            break;
        default:
            AttachToFramebuffer();      //attch to color buffer
            break;
        }
    }
}

void RenderTexture::AttachToFramebuffer(FrameBufferObject::Ptr framebuf
    , AttachmentType type
    , int32 index)
{
    if(framebuf != nullptr && _framebuf == nullptr)
    {
        _framebuf = framebuf;

        AttachToFramebuffer(type, index);
    }
}

void RenderTexture::AttachToFramebuffer(AttachmentType type, int32 index)
{
    _framebuf->Bind();
    if (_texture != nullptr)
    {
        switch (type)
        {
        case AttachmentType::COLOR_ATTACHMENT:
            _framebuf->AttachColor(_texture, index);
            break;
        case AttachmentType::DEPTH_ATTACHMENT:
            _framebuf->AttachDepth(_texture);
            break;
        case AttachmentType::DEPTH_STENCIL:
            _framebuf->AttachDepthStencil(_texture);
            break;
        default:
            break;
        }
    }
    if (_usedepth)
    {
        if (_depthwithstencil)
        {
            _framebuf->AttachDepthStencil(_depthbuf);
        }
        else
        {
            _framebuf->AttachDepth(_depthbuf);
        }
    }
    _framebuf->UnBind();
}

} // namespace TwinkleGraphics
