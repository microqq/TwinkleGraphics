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

    enum class RTType
    {
        COLOR,
        DEPTH,
        DEPTHSTENCIL
    };

    RenderTexture(int32 width, int32 height, GLenum internalformat = GL_RGBA8
        , GLenum format = GL_RGBA
        , bool usedepth = true, bool depthwithstencil = false
        , bool antialiasing = false, int32 samples = 1
        , bool fixedsampledlocation = true);
    virtual ~RenderTexture();

    /**
     * @brief 
     * 
     * @param autogenFBO 
     */
    void Create(bool autogenFBO = true);
    Texture::Ptr GetTexture() { return _texture; }
    RenderBufferObject::Ptr GetDepthBuffer() { return _depthbuf; }
    FrameBufferObject::Ptr GetFrameBuffer() { return _framebuf; }
    void Bind() { if(_framebuf != nullptr) _framebuf->Bind(); }
    void UnBind() { if(_framebuf != nullptr) _framebuf->UnBind(); }

    void AttachToFrameBuffer();


private:
    void AttachToFramebuffer(AttachmentType type, int32 index = 0);

private:
    Texture::Ptr _texture;
    RenderBufferObject::Ptr _depthbuf;
    FrameBufferObject::Ptr _framebuf;

    int32 _width;
    int32 _height;
    int32 _samples;

    RTType _rttype;;
    GLenum _internalformat;
    GLenum _format;

    bool _usedepth;
    bool _depthwithstencil;
    bool _antialiasing;
    bool _fixedsampledlocation;
};

} // namespace TwinkleGraphics



#endif