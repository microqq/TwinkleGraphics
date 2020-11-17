#ifndef TW_RENDERTEXTURE_H
#define TW_RENDERTEXTURE_H

#include "twTexture.h"
#include "twHWBuffers.h"

namespace TwinkleGraphics
{

class __TWCOMExport RenderTexture : public Object
{
public:
    typedef std::shared_ptr<RenderTexture> Ptr;
    using AttachmentType = FrameBufferObject::AttachmentType;

    RenderTexture(int32 width, int32 height, GLenum internalformat = GL_RGBA8
        , GLenum format = GL_RGBA
        , bool usedepth = true, bool depthwithstencil = false
        , bool multisample = false, int32 samples = 1
        , bool fixedsampledlocation = true);
    virtual ~RenderTexture();

    void Create(FrameBufferObject::Ptr framebuf = nullptr);
    void Resize(int32 width, int32 height);
    int32 GetWidth() { return _width; }
    int32 GetHeight() { return _height; }
    Texture::Ptr GetTexture() { return _texture; }
    RenderBufferObject::Ptr GetDepthBuffer() { return _depthbuf; }
    FrameBufferObject::Ptr GetFrameBuffer() { return _framebuf; }
    
    void Bind() { assert(_framebuf != nullptr); _framebuf->Bind(); }
    void UnBind() { assert(_framebuf != nullptr); _framebuf->UnBind(); }

    void BlitColor(RenderTexture::Ptr dest);
    void BlitDepth(RenderTexture::Ptr dest);

    void BlitColorToBackBuffer(int backwidth, int backheight);
    void BlitDepthToBackBuffer(int backwidth, int backheight);

private:
    Texture::Ptr _texture = nullptr;
    RenderBufferObject::Ptr _depthbuf = nullptr;
    FrameBufferObject::Ptr _framebuf = nullptr;

    int32 _width;
    int32 _height;
    int32 _samples;

    GLenum _internalformat;
    GLenum _format;

    bool _usedepth;
    bool _depthwithstencil;
    bool _multisample;
    bool _fixedsampledlocation;
};

} // namespace TwinkleGraphics



#endif