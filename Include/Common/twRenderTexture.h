#ifndef TW_RENDERTEXTURE_H
#define TW_RENDERTEXTURE_H

#include "twHWBuffers.h"
#include "twTexture.h"


namespace TwinkleGraphics {

class __TWAPI RenderTexture : public Object {
public:
  using Ptr = std::shared_ptr<RenderTexture> ;
  using AttachmentType = FrameBufferObject::AttachmentType;

  RenderTexture(int32 width, int32 height, GLenum internalformat = GL_RGBA8,
                GLenum format = GL_RGBA, bool usedepth = true,
                bool depthwithstencil = false, bool multisample = false,
                int32 samples = 1, bool fixedsampledlocation = true);
  virtual ~RenderTexture();

  void Create(FrameBufferObjectPtr framebuf = nullptr);
  void Resize(int32 width, int32 height);
  int32 GetWidth() { return _width; }
  int32 GetHeight() { return _height; }
  TexturePtr GetTexture() { return _texture; }
  RenderBufferObjectPtr GetDepthBuffer() { return _depthbuf; }
  FrameBufferObjectPtr GetFrameBuffer() { return _framebuf; }

  void Bind() {
    assert(_framebuf != nullptr);
    _framebuf->Bind();
  }
  void UnBind() {
    assert(_framebuf != nullptr);
    _framebuf->UnBind();
  }

  void BlitColor(RenderTexture::Ptr dest);
  void BlitDepth(RenderTexture::Ptr dest);

  void BlitColorToBackBuffer(int backwidth, int backheight);
  void BlitDepthToBackBuffer(int backwidth, int backheight);

private:
  TexturePtr _texture = nullptr;
  RenderBufferObjectPtr _depthbuf = nullptr;
  FrameBufferObjectPtr _framebuf = nullptr;

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

using RenderTexturePtr = RenderTexture::Ptr;

} // namespace TwinkleGraphics

#endif