

#ifndef TW_HWBUFFER_H
#define TW_HWBUFFER_H

#include "twCommon.h"
#include "twHWObject.h"
#include "twTexture.h"
#include <GL/glew.h>

namespace TwinkleGraphics {
class IHWBuffer : public IHWObject {
public:
  using Ptr = std::shared_ptr<IHWBuffer>;

  enum MapAccessType {
    MAP_READ = GL_MAP_READ_BIT,
    MAP_WRITE = GL_MAP_WRITE_BIT,
    MAP_READ_WRITE = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT,
    // Option flag
    INVALIDATE_RANGE = GL_MAP_INVALIDATE_RANGE_BIT,
    INVALIDATE_BUFFER = GL_MAP_INVALIDATE_BUFFER_BIT,
    FLUSH_EXPLICIT = GL_MAP_FLUSH_EXPLICIT_BIT,
    UNSYNCHRONIZED = GL_MAP_UNSYNCHRONIZED_BIT
  };

  IHWBuffer(int32 type) : IHWObject(type) {}
  virtual ~IHWBuffer() {}

  virtual void Create() override {
    uint32 *bufs = new uint32[1];
    glGenBuffers(1, bufs);

    _resinstance.id = bufs[0];
    SAFE_DEL_ARR(bufs);
  }

  virtual void Resize(int32 width, int32 height) {}

  virtual void InitBufferData(uint32 size, const void *data) {
    glBufferData(_resinstance.type, size, data, GL_DYNAMIC_DRAW);

    _bufsize = size;
  }

  virtual void UpdateBufferData(uint32 offset, uint32 size, const void *data) {
    assert(data != nullptr && size + offset <= _bufsize);

    if (offset == 0 && size == _bufsize) {
      glBufferData(_resinstance.type, size, data, GL_DYNAMIC_DRAW);
      return;
    }

    glBufferSubData(_resinstance.type, offset, size, data);
  }

  virtual void *MapBuffer(uint32 offset, uint32 size, MapAccessType access) {
    assert(size + offset <= _bufsize);

    return glMapBufferRange(_resinstance.type, offset, size, access);
  }

  virtual void UnMapBuffer() {
    glBindBuffer(_resinstance.type, _resinstance.id);
    glUnmapBuffer(_resinstance.type);
  }

  virtual void Destroy() override {
    if (_resinstance.id != 0) {
      glBindBuffer(_resinstance.type, 0);

      uint32 bufs[1] = {_resinstance.id};
      glDeleteBuffers(1, bufs);
    }
  }

  virtual void Bind() override {
    glBindBuffer(_resinstance.type, _resinstance.id);
  }

  virtual void UnBind() override { glBindBuffer(_resinstance.type, 0); }

  virtual int32 GetBufferSize() { return _bufsize; }

protected:
  uint32 _bufsize;
};

/*------------------------------Vertex Array Object--------------------------*/

class VertexArrayObject : public IHWObject {
public:
  using Ptr = std::shared_ptr<VertexArrayObject>;

  VertexArrayObject() : IHWObject(GL_VERTEX_ARRAY) { Create(); }

  virtual ~VertexArrayObject() { Destroy(); }

  virtual void Create() override {
    uint32 *vaos = new uint32[1];
    glGenVertexArrays(1, vaos);

    _resinstance.id = vaos[0];
    SAFE_DEL_ARR(vaos);
  }
  virtual void Destroy() override {
    if (_resinstance.id != 0) {
      glBindVertexArray(0);

      uint32 vaos[1] = {_resinstance.id};
      glDeleteVertexArrays(1, vaos);
    }
  }
  virtual void Bind() override { glBindVertexArray(_resinstance.id); }
  virtual void UnBind() override { glBindVertexArray(0); }
};

using VertexArrayObjectPtr = VertexArrayObject::Ptr;

/*------------------------------Vertex Buffer--------------------------*/

class VertexBufferObject : public IHWBuffer {
public:
  using Ptr = std::shared_ptr<VertexBufferObject>;

  VertexBufferObject() : IHWBuffer(GL_ARRAY_BUFFER) { Create(); }

  virtual ~VertexBufferObject() { Destroy(); }
};

using VertexBufferObjectPtr = VertexBufferObject::Ptr;

/*------------------------------Index Buffer--------------------------*/

class IndexBufferObject : public IHWBuffer {
public:
  using Ptr = std::shared_ptr<IndexBufferObject>;

  IndexBufferObject() : IHWBuffer(GL_ELEMENT_ARRAY_BUFFER) { Create(); }

  virtual ~IndexBufferObject() { Destroy(); }
};

using IndexBufferObjectPtr = IndexBufferObject::Ptr;

/*------------------------------RenderBuffer--------------------------*/

class RenderBufferObject : public IHWBuffer {
public:
  using Ptr = std::shared_ptr<RenderBufferObject>;

  RenderBufferObject(int32 width, int32 height, GLenum internalformat,
                     int32 samples = 1, bool multisample = false)
      : IHWBuffer(GL_RENDERBUFFER), _width(width), _height(height),
        _samples(samples), _internalformat(internalformat),
        _multisample(multisample) {
    Create();
  }
  virtual ~RenderBufferObject() { Destroy(); }

  virtual void Create() override {
    assert(_width > 0 && _height > 0);

    uint32 bufs[1];
    glGenRenderbuffers(1, bufs);
    _resinstance.id = bufs[0];

    Bind();
    if (_multisample) {
      glRenderbufferStorageMultisample(GL_RENDERBUFFER, _samples,
                                       _internalformat, _width, _height);
    } else {
      glRenderbufferStorage(GL_RENDERBUFFER, _internalformat, _width, _height);
    }
    UnBind();
  }

  virtual void Resize(int32 width, int32 height) override {
    _width = width;
    _height = height;
    Bind();
    if (_multisample) {
      glRenderbufferStorageMultisample(GL_RENDERBUFFER, _samples,
                                       _internalformat, _width, _height);
    } else {
      glRenderbufferStorage(GL_RENDERBUFFER, _internalformat, _width, _height);
    }
    UnBind();
  }

  virtual void Destroy() override {
    uint32 bufs[1] = {_resinstance.id};
    glDeleteRenderbuffers(1, bufs);
  }
  virtual void Bind() override {
    glBindRenderbuffer(GL_RENDERBUFFER, _resinstance.id);
  }
  virtual void UnBind() override { glBindRenderbuffer(GL_RENDERBUFFER, 0); }

private:
  int32 _width;
  int32 _height;
  int32 _samples;

  GLenum _internalformat;
  bool _multisample;
};
using RenderBufferObjectPtr = RenderBufferObject::Ptr;

/*------------------------------FrameBuffer--------------------------*/

class FrameBufferObject : public IHWBuffer {
public:
  using Ptr = std::shared_ptr<FrameBufferObject>;

  enum AttachmentType {
    COLOR_ATTACHMENT = GL_COLOR_ATTACHMENT0,
    DEPTH_ATTACHMENT = GL_DEPTH_ATTACHMENT,
    STENCIL_ATTACHMENT = GL_STENCIL_ATTACHMENT,
    DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT
  };

  FrameBufferObject(uint32 type = GL_FRAMEBUFFER)
      : IHWBuffer(type), _colorAttachments(0) {
    Create();
  }
  virtual ~FrameBufferObject() { Destroy(); }

  virtual void Create() override {
    uint32 bufs[1];
    glGenFramebuffers(1, bufs);

    _resinstance.id = bufs[0];
  }
  virtual void Destroy() override {
    uint32 bufs[1] = {_resinstance.id};
    glDeleteFramebuffers(1, bufs);
  }
  virtual void Bind() override {
    glBindFramebuffer(_resinstance.type, _resinstance.id);
  }
  virtual void UnBind() override { glBindFramebuffer(_resinstance.type, 0); }

  void BlitColorTo(int src_width, int src_height, FrameBufferObject::Ptr dest,
                   int dest_width, int dest_height, GLenum filter) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _resinstance.id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dest->_resinstance.id);

    glBlitFramebuffer(0, 0, src_width, src_height, 0, 0, dest_width,
                      dest_height, GL_COLOR_BUFFER_BIT, filter);
  }

  void BlitColorToBack(int src_width, int src_height, int dest_width,
                       int dest_height, GLenum filter) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _resinstance.id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glBlitFramebuffer(0, 0, src_width, src_height, 0, 0, dest_width,
                      dest_height, GL_COLOR_BUFFER_BIT, filter);
  }

  void BlitDepthTo(int src_width, int src_height, FrameBufferObject::Ptr dest,
                   int dest_width, int dest_height, GLenum filter) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _resinstance.id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dest->_resinstance.id);

    glBlitFramebuffer(0, 0, src_width, src_height, 0, 0, dest_width,
                      dest_height, GL_DEPTH_BUFFER_BIT, filter);
  }

  void BlitDepthToBack(int src_width, int src_height, int dest_width,
                       int dest_height, GLenum filter) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _resinstance.id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glBlitFramebuffer(0, 0, src_width, src_height, 0, 0, dest_width,
                      dest_height, GL_DEPTH_BUFFER_BIT, filter);
  }

  void AttachColor(TexturePtr tex, int32 index = 0) {
    assert(index >= 0 && index < 16);

    int attachindex = 0;
    if (index >= _colorAttachments) {
      attachindex = _colorAttachments;
      _colorAttachments += 1;
    } else {
      attachindex = index;
    }

    const RenderResourceHandle &res = tex->GetTexResource();
    glFramebufferTexture2D(_resinstance.type,
                           AttachmentType::COLOR_ATTACHMENT + attachindex,
                           res.type, res.id, 0);
  }
  void AttachColor(RenderBufferObjectPtr rbobj, int32 index = 0) {
    assert(index >= 0 && index < 16);

    int attachindex = 0;
    if (index >= _colorAttachments) {
      attachindex = _colorAttachments;
      _colorAttachments += 1;
    } else {
      attachindex = index;
    }

    const RenderResourceHandle &res = rbobj->GetResource();
    glFramebufferRenderbuffer(_resinstance.type,
                              AttachmentType::COLOR_ATTACHMENT + attachindex,
                              res.type, res.id);
  }
  void AttachDepth(TexturePtr tex) {
    const RenderResourceHandle &res = tex->GetTexResource();
    glFramebufferTexture2D(_resinstance.type, AttachmentType::DEPTH_ATTACHMENT,
                           res.type, res.id, 0);
  }
  void AttachDepth(RenderBufferObjectPtr rbobj) {
    const RenderResourceHandle &res = rbobj->GetResource();
    glFramebufferRenderbuffer(
        _resinstance.type, AttachmentType::DEPTH_ATTACHMENT, res.type, res.id);
  }
  void AttachStencil(RenderBufferObjectPtr rbobj) {
    const RenderResourceHandle &res = rbobj->GetResource();
    glFramebufferRenderbuffer(_resinstance.type,
                              AttachmentType::STENCIL_ATTACHMENT, res.type,
                              res.id);
  }
  void AttachDepthStencil(TexturePtr tex) {
    const RenderResourceHandle &res = tex->GetTexResource();
    glFramebufferTexture2D(_resinstance.type, AttachmentType::DEPTH_STENCIL,
                           res.type, res.id, 0);
  }
  void AttachDepthStencil(RenderBufferObjectPtr rbobj) {
    const RenderResourceHandle &res = rbobj->GetResource();
    glFramebufferRenderbuffer(_resinstance.type, AttachmentType::DEPTH_STENCIL,
                              res.type, res.id);
  }

  void DrawColorBuffers() {
    if (_colorAttachments > 1) {
      uint32 *attachments = new uint32[_colorAttachments];
      for (int32 i = 0; i < _colorAttachments; i++) {
        attachments[i] = AttachmentType::COLOR_ATTACHMENT + i;
      }
      glDrawBuffers(_colorAttachments, attachments);
    }
  }
  void CheckFramebuffer() {}
  int32 GetColorAttachmentsCount() { return _colorAttachments; }

private:
  int32 _colorAttachments = 0;
};

using FrameBufferObjectPtr = FrameBufferObject::Ptr;

} // namespace TwinkleGraphics

#endif