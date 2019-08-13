

#ifndef TW_TEXTURE_H
#define TW_TEXTURE_H

#include "twCommon.h"

#include "twRenderContext.h"
#include "twImage.h"

namespace TwinkleGraphics
{
typedef GLenum InternalFormat;


typedef Swizzle<1> Swizzle1;
typedef Swizzle<4> Swizzle4;

struct TexParams
{
    WrapMode wrap_s;
    WrapMode wrap_t;
    WrapMode wrap_r;

    FilterMode filter_min;
    FilterMode filter_mag;

    SwizzleParam swizzle_parameter;
    SwizzleMask swizzle[4];

    LodBiasParam lod_parameter;
    float32 lodbias;

    TexParams()
        : wrap_s(WrapMode::NONE)
        , wrap_t(WrapMode::NONE)
        , wrap_r(WrapMode::NONE)
        , filter_min(FilterMode::NONE)
        , filter_mag(FilterMode::NONE)
        , swizzle_parameter(SwizzleParam::NONE)
        , lod_parameter(LodBiasParam::NONE)
    {
    }
};

/**
 * @brief 
 * "A GL texture object includes both categories. The first category represents dimensionality and other image parameters, 
 *  and the second category represents sampling state." ---- OpenGL 4.5(Core Profile)
 */
class Texture : public Object
{
public:
    typedef std::shared_ptr<Texture> Ptr;

    Texture(bool immutable = true);
    virtual ~Texture();

    void SetImage(Image::Ptr image) 
    { 
        if(_image == nullptr) 
        {
#ifdef _DEBUG            
            assert(image->GetImageSource().target == _res.type);
#endif
            _image = image;
            InitStorage();
        }
        else if(!_immutable)
        {
            //update texture storage
        }
    }

    template<WrapParam Wrap>
    void SetWrap(WrapMode wrap);
    template<FilterParam Filter>
    void SetFilter(FilterMode filter);
    template<int32 L>
    void SetSwizzle(SwizzleParam parameter, Swizzle<L> swizzle);
    void SetLodBias(LodBiasParam parameter, float32 bias);

    Image::Ptr GetImage() { return _image; }
    const TexParams& GetTexParams() { return _parameters; }
    int32 GetNumMipLevels() { return _image == nullptr ? 0 : _image->GetImageSource().mipLevels; }
    InternalFormat GetInternalformat() { return _image == nullptr ? GL_NONE : _image->GetImageSource().internalFormat; }

    int32 GetWidth(int32 level) { return _image->GetImageSource().mip[level].width; }
    int32 GetHeight(int32 level) { return _image->GetImageSource().mip[level].height; }
    int32 GetDepth(int32 level = 0) { return _image->GetImageSource().mip[level].depth; }

    const RenderResInstance& GetRenderRes() { return _res; }

    void Apply();

protected:
    virtual void InitStorage();
    virtual void ApplyTexParams();
    

protected:
    TexParams _parameters;
    RenderResInstance _res;
    Image::Ptr _image;

    bool _immutable;
    bool _initialized;
};

class Texture1D : public Texture
{
public:
    typedef std::shared_ptr<Texture1D> Ptr;

    Texture1D(bool immutable = true)
        : Texture(immutable)
    {
        _res.type = GL_TEXTURE_1D;
    }
    virtual ~Texture1D() {}

protected:
    virtual void InitStorage() override;
};

class Texture2D : public Texture
{
public:
    typedef std::shared_ptr<Texture2D> Ptr;

    Texture2D(bool immutable = true)
        : Texture(immutable)
    {
        _res.type = GL_TEXTURE_2D;
    }
    virtual ~Texture2D() {}

protected:
    virtual void InitStorage() override;

};

class Texture2DMultiSample : public Texture
{
public:
    typedef std::shared_ptr<Texture2DMultiSample> Ptr;

    Texture2DMultiSample(bool immutable = true)
            : Texture(immutable)
    {
        _res.type = GL_TEXTURE_2D_MULTISAMPLE;
    }

    virtual ~Texture2DMultiSample() {}

protected:
    virtual void InitStorage() override;

};

class Texture3D : public Texture
{
public:
    typedef std::shared_ptr<Texture3D> Ptr;

    Texture3D(bool immutable = true)
            : Texture(immutable)
    {
        _res.type = GL_TEXTURE_3D;
    }

    virtual ~Texture3D() {}

protected:
    virtual void InitStorage() override;
};

class TextureRectangle : public Texture
{
public:
    typedef std::shared_ptr<TextureRectangle> Ptr;

    TextureRectangle(bool immutable = true)
            : Texture(immutable)
    {
        _res.type = GL_TEXTURE_RECTANGLE;
    }

    virtual ~TextureRectangle() {}

protected:
    virtual void InitStorage() override;
};

class TextureBuffer : public Texture
{
public:
    typedef std::shared_ptr<TextureBuffer> Ptr;

    TextureBuffer(bool immutable = true)
            : Texture(immutable)
    {
        _res.type = GL_TEXTURE_BUFFER;
    }

    virtual ~TextureBuffer() {}

protected:
    virtual void InitStorage() override;
};

class TextureCube : public Texture
{
public:
    typedef std::shared_ptr<TextureCube> Ptr;

    TextureCube()
        : Texture(false)
    {
        _res.type = GL_TEXTURE_CUBE_MAP;
    }
    virtual ~TextureCube() {}

protected:
    virtual void InitStorage() override;
};

class Texture1DArray : public Texture
{
public:
    typedef std::shared_ptr<Texture1DArray> Ptr;

    Texture1DArray(bool immutable = true)
            : Texture(immutable)
    {
        _res.type = GL_TEXTURE_1D_ARRAY;
    }

    virtual ~Texture1DArray() {}

protected:
    virtual void InitStorage() override;
};

class Texture2DArray : public Texture
{
public:
    typedef std::shared_ptr<Texture2DArray> Ptr;

    Texture2DArray(bool immutable = true)
            : Texture(immutable)
    {
        _res.type = GL_TEXTURE_2D_ARRAY;
    }

    virtual ~Texture2DArray() {}

protected:
    virtual void InitStorage() override;
};

class TextureCubeArray : public Texture
{
public:
    typedef std::shared_ptr<TextureCubeArray> Ptr;

    TextureCubeArray(bool immutable = true)
            : Texture(immutable)
    {
        _res.type = GL_TEXTURE_CUBE_MAP_ARRAY;
    }

    virtual ~TextureCubeArray() {}

protected:
    virtual void InitStorage() override;
};

class Texture2DMultiSampleArray : public Texture
{
public:
    typedef std::shared_ptr<Texture2DMultiSampleArray> Ptr;

    Texture2DMultiSampleArray(bool immutable = true)
            : Texture(immutable)
    {
        _res.type = GL_TEXTURE_2D_MULTISAMPLE_ARRAY;        
    }

    virtual ~Texture2DMultiSampleArray() {}

protected:
    virtual void InitStorage() override;
};

class TextureManager;
typedef Singleton<TextureManager> TextureManagerInst;

class TextureManager
{
public:
    TextureManager()
    {}
    ~TextureManager()
    {}

private:

};

} // namespace TwinkleGraphics

#endif