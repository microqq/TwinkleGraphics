

#include "twRenderTexture.h"
#include "twConsoleLog.h"

namespace TwinkleGraphics
{
RenderTexture::RenderTexture(int32 width, int32 height
        // , RTType type = RTType::COLOR
        , GLenum internalformat, GLenum format
        , bool usedepth, bool depthwithstencil
        , bool multisample, int32 samples
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
    , _multisample(multisample)
    , _fixedsampledlocation(fixedsampledlocation)
{
}

RenderTexture::~RenderTexture()
{}

void RenderTexture::Create(FrameBufferObject::Ptr framebuf)
{
    if(framebuf == nullptr)
    {
        _framebuf = std::make_shared<FrameBufferObject>();
    }
    else
    {
        _framebuf = framebuf;        
    }
    _framebuf->Bind();

    if(!_multisample)
    {
        _texture = std::make_shared<Texture2D>();
        _texture->Create(_width, _height, _internalformat, _format);
    }
    else
    {
        _texture = std::make_shared<Texture2DMultiSample>(_samples);
        _texture->Create(_width, _height, _internalformat, _format);
    }

    switch (_format)
    {
    case GL_DEPTH_COMPONENT:
        _framebuf->AttachDepth(_texture);
        break;
    case GL_DEPTH_STENCIL:
        _framebuf->AttachDepthStencil(_texture);
        break;
    case GL_STENCIL_INDEX:
        break;
    default:
        int index = _framebuf->GetColorAttachmentsCount();
        _framebuf->AttachColor(_texture, index);
        break;
    }    

    if(_usedepth)
    {
        if(_depthwithstencil)
        {
            _depthbuf = std::make_shared<RenderBufferObject>(_width, _height, GL_DEPTH24_STENCIL8, _samples, _multisample);
            _framebuf->AttachDepthStencil(_depthbuf);
        }
        else
        {
            _depthbuf = std::make_shared<RenderBufferObject>(_width, _height, GL_DEPTH_COMPONENT, _samples, _multisample);
            _framebuf->AttachDepth(_depthbuf);
        }
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        Console::LogError("FrameBuffer: Framebuffer is not complete!\n");
    }

    _framebuf->UnBind();
}


void RenderTexture::BlitColor(RenderTexture::Ptr dest)
{
    assert(_framebuf != nullptr);

    _framebuf->BlitColorTo(_width, _height, dest->_framebuf, dest->_width, dest->_height, GL_NEAREST);
}

void RenderTexture::BlitDepth(RenderTexture::Ptr dest)
{
    assert(_framebuf != nullptr);

    _framebuf->BlitDepthTo(_width, _height, dest->_framebuf, dest->_width, dest->_height, GL_NEAREST);
}

void RenderTexture::BlitColorToBackBuffer(int backwidth, int backheight)
{
    _framebuf->BlitColorToBack(_width, _height, backwidth, backheight, GL_NEAREST);
}

void RenderTexture::BlitDepthToBackBuffer(int backwidth, int backheight)
{
    _framebuf->BlitDepthToBack(_width, _height, backwidth, backheight, GL_NEAREST);
}


} // namespace TwinkleGraphics
