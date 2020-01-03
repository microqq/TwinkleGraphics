

#ifndef TW_HWBUFFER_H
#define TW_HWBUFFER_H

#include <glew/glew.h>
#include "twCommon.h"
#include "twHWObject.h"
#include "twTexture.h"

namespace TwinkleGraphics
{
class IHWBuffer : public IHWObject
{
public:
    typedef std::shared_ptr<IHWBuffer> Ptr;

    enum MapAccessType
    {
        MAP_READ = GL_MAP_READ_BIT,
        MAP_WRITE = GL_MAP_WRITE_BIT,
        MAP_READ_WRITE = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT,
        // Option flag
        INVALIDATE_RANGE = GL_MAP_INVALIDATE_RANGE_BIT,
        INVALIDATE_BUFFER = GL_MAP_INVALIDATE_BUFFER_BIT,
        FLUSH_EXPLICIT = GL_MAP_FLUSH_EXPLICIT_BIT,
        UNSYNCHRONIZED = GL_MAP_UNSYNCHRONIZED_BIT
    };


    IHWBuffer(int32 type)
        : IHWObject(type)
    {}
    virtual ~IHWBuffer() {}

    virtual void Create() override
    {
        uint32 bufs[1];
        glGenBuffers(_resinstance.type, bufs);

        _resinstance.id = bufs[0];
    }

    virtual void InitBufferData(uint32 size, const void *data)
    {
        glBufferData(_resinstance.type, size, data, GL_DYNAMIC_DRAW);

        _bufsize = size;
    }

    virtual void UpdateBufferData(uint32 offset, uint32 size, const void *data)
    {
        assert(data != nullptr && size + offset <= _bufsize);

        if(offset == 0 && size == _bufsize)
        {
            glBufferData(_resinstance.type, size, data, GL_DYNAMIC_DRAW);
            return;
        }
    
        glBufferSubData(_resinstance.type, offset, size, data);
    }

    virtual void* MapBuffer(uint32 offset, uint32 size, MapAccessType access)
    {
        assert(size + offset <= _bufsize);
        
        return glMapBufferRange(_resinstance.type, offset, size, access);
    }

    virtual void UnMapBuffer()
    {
        glBindBuffer(_resinstance.type, _resinstance.id);
        glUnmapBuffer(_resinstance.type);
    }

    virtual void Destroy() override
    {
        if(_resinstance.id != 0)
        {
            glBindBuffer(_resinstance.type, 0);

            uint32 bufs[1] = { _resinstance.id };
            glDeleteBuffers(1, bufs);
        }
    }

    virtual void Bind() override
    {
        glBindBuffer(_resinstance.type, _resinstance.id);
    }

    virtual void UnBind() override
    {
        glBindBuffer(_resinstance.type, 0);
    }

    virtual int32 GetBufferSize() { return _bufsize; }

protected:
    uint32 _bufsize;
};


/*------------------------------Vertex Array Object--------------------------*/

class VertexArrayObject : public IHWObject
{
public:
    typedef std::shared_ptr<VertexArrayObject> Ptr;

    VertexArrayObject()
        : IHWObject(GL_VERTEX_ARRAY)
    {
        Create();
    }

    virtual ~VertexArrayObject()
    {
        Destroy();
    }

    virtual void Create() override
    {
        uint32 vaos[1];
        glGenVertexArrays(1, vaos);

        _resinstance.id = vaos[0];
    }
    virtual void Destroy() override
    {
        if (_resinstance.id != 0)
        {
            glBindVertexArray(0);

            uint32 vaos[1] = { _resinstance.id };
            glDeleteVertexArrays(1, vaos);
        }
    }
    virtual void Bind() override
    {
        glBindVertexArray(_resinstance.id);
    }
    virtual void UnBind() override
    {
        glBindVertexArray(0);
    }
};



/*------------------------------Vertex Buffer--------------------------*/

class VertexBufferObject : public IHWBuffer
{
public:
    typedef std::shared_ptr<VertexBufferObject> Ptr;

    VertexBufferObject()
        : IHWBuffer(GL_ARRAY_BUFFER)
    {
        Create();
    }

    virtual ~VertexBufferObject()
    {
        Destroy();
    }
};



/*------------------------------Index Buffer--------------------------*/

class IndexBufferObject : public IHWBuffer
{
public:
    typedef std::shared_ptr<IndexBufferObject> Ptr;

    IndexBufferObject()
        : IHWBuffer(GL_ELEMENT_ARRAY_BUFFER)
    {
        Create();
    }

    virtual ~IndexBufferObject()
    {
        Destroy();
    }
};


/*------------------------------RenderBuffer--------------------------*/

class RenderBufferObject : public IHWBuffer
{
public:
    typedef std::shared_ptr<RenderBufferObject> Ptr;

    RenderBufferObject(int32 width, int32 height, GLenum internalformat, int32 samples = 1, bool antialiasing = false)
        : IHWBuffer(GL_RENDERBUFFER)
        , _width(width)
        , _height(height)
        , _samples(samples)
        , _internalformat(internalformat)
        , _antialiasing(antialiasing)
    {
        Create();
    }
    virtual ~RenderBufferObject() 
    {
        Destroy();
    }

    virtual void Create() override 
    {
        assert(_width > 0 && _height > 0);

        uint32 bufs[1];
        glGenRenderbuffers(1, bufs);
        _resinstance.id = bufs[0];

        Bind();
        if(_antialiasing)
        {
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, _samples, _internalformat, _width, _height);
        }
        else
        {
            glRenderbufferStorage(GL_RENDERBUFFER, _internalformat, _width, _height);
        }
        UnBind();
    }
    virtual void Destroy() override 
    {
        uint32 bufs[1] = { _resinstance.id };
        glDeleteRenderbuffers(1, bufs);
    }
    virtual void Bind() override 
    {
        glBindRenderbuffer(GL_RENDERBUFFER, _resinstance.id);
    }
    virtual void UnBind() override
    {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);        
    }

private:
    int32 _width;
    int32 _height;
    int32 _samples;

    GLenum _internalformat;
    bool _antialiasing;
};


/*------------------------------FrameBuffer--------------------------*/

class FrameBufferObject : public IHWBuffer
{
public:
    typedef std::shared_ptr<FrameBufferObject> Ptr;

    enum AttachmentType
    {
        COLOR_ATTACHMENT = GL_COLOR_ATTACHMENT0,
        DEPTH_ATTACHMENT = GL_DEPTH_ATTACHMENT,
        STENCIL_ATTACHMENT = GL_STENCIL_ATTACHMENT,
        DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT
    };

    FrameBufferObject(uint32 type = GL_DRAW_FRAMEBUFFER)
        : IHWBuffer(type)
        , _color_attachments(0)
    {
        Create();
    }
    virtual ~FrameBufferObject() 
    {
        Destroy();
    }

    virtual void Create() override
    {
        uint32 bufs[1];
        glGenFramebuffers(1, bufs);

        _resinstance.id = bufs[0];
    }
    virtual void Destroy() override 
    {
        uint32 bufs[1] = { _resinstance.id };
        glDeleteFramebuffers(1, bufs);
    }
    virtual void Bind() override
    {
        glBindFramebuffer(_resinstance.type, _resinstance.id);
    }
    virtual void UnBind() override
    {
        glBindFramebuffer(_resinstance.type, 0);
    }

    void AttachColor(Texture::Ptr tex, int32 index = 0)
    {
        const RenderResInstance& res = tex->GetTexResource();
        glFramebufferTexture2D(_resinstance.type, AttachmentType::COLOR_ATTACHMENT + index, res.type, res.id, 0);

        if(index >= _color_attachments)
            _color_attachments += 1;
    }
    void AttachColor(RenderBufferObject::Ptr rbobj, int32 index = 0)
    {
        const RenderResInstance& res = rbobj->GetResource();
        glFramebufferRenderbuffer(_resinstance.type, AttachmentType::COLOR_ATTACHMENT + index, res.type, res.id);

        if(index >= _color_attachments)
            _color_attachments += 1;
    }
    void AttachDepth(Texture::Ptr tex)
    {
        const RenderResInstance& res = tex->GetTexResource();
        glFramebufferTexture2D(_resinstance.type, AttachmentType::DEPTH_ATTACHMENT, res.type, res.id, 0);
    }
    void AttachDepth(RenderBufferObject::Ptr rbobj)
    {
        const RenderResInstance& res = rbobj->GetResource();
        glFramebufferRenderbuffer(_resinstance.type, AttachmentType::DEPTH_ATTACHMENT, res.type, res.id);
    }
    void AttachStencil(RenderBufferObject::Ptr rbobj)
    {
        const RenderResInstance& res = rbobj->GetResource();
        glFramebufferRenderbuffer(_resinstance.type, AttachmentType::STENCIL_ATTACHMENT, res.type, res.id);
    }
    void AttachDepthStencil(Texture::Ptr tex)
    {
        const RenderResInstance& res = tex->GetTexResource();
        glFramebufferTexture2D(_resinstance.type, AttachmentType::DEPTH_STENCIL, res.type, res.id, 0);
    }
    void AttachDepthStencil(RenderBufferObject::Ptr rbobj)
    {
        const RenderResInstance& res = rbobj->GetResource();
        glFramebufferRenderbuffer(_resinstance.type, AttachmentType::DEPTH_STENCIL, res.type, res.id);
    }

    void DrawColorBuffers() 
    {
        if(_color_attachments > 1)
        {
            uint32 attachments[_color_attachments];
            for(int32 i = 0; i < _color_attachments; i++)
            {
                attachments[i] = AttachmentType::COLOR_ATTACHMENT + i;
            }
            glDrawBuffers(_color_attachments, attachments);
        }
    }
    void CheckFramebuffer() {}
    int32 GetColorAttachmentsCount() { return _color_attachments; }

private:
    int32 _color_attachments;
};

} // namespace TwinkleGraphics


#endif