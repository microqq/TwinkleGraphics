#ifndef TW_RENDERTEXTURE_H
#define TW_RENDERTEXTURE_H

#include "twTexture.h"
#include "twHWBuffers.h"

namespace TwinkleGraphics
{

class RenderTexture : public Object
{
public:
    typedef std::shared_ptr<RenderTexture> Ptr;
    using AttachmentType = FrameBufferObject::AttachmentType;

    RenderTexture(int32 width, int32 height, GLenum internalformat, GLenum format
        , bool usedepth = true, bool depthwithstencil = false
        , bool antialiasing = false, int32 samples = 1
        , bool fixedsampledlocation = true);
    virtual ~RenderTexture();

    void Create();
    void AttachToFramebuffer(FrameBufferObject::Ptr framebuf
        , AttachmentType type = AttachmentType::COLOR_ATTACHMENT
        ,  bool genframebuf = false);

    Texture::Ptr GetTexture() { return _texture; }

private:
    Texture::Ptr _texture;
    RenderBufferObject::Ptr _depthbuf;
    FrameBufferObject::Ptr _framebuf;

    int32 _width;
    int32 _height;
    int32 _samples;

    GLenum _internalformat;
    GLenum _format;

    bool _usedepth;
    bool _depthwithstencil;
    bool _antialiasing;
    bool _fixedsampledlocation;
};

} // namespace TwinkleGraphics



#endif