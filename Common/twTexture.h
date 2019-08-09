

#ifndef TW_TEXTURE_H
#define TW_TEXTURE_H

#include "twCommon.h"

#include "twImage.h"

namespace TwinkleGraphics
{
typedef GLenum Internalformat;

class TextureManager
{
public:
    TextureManager();
    ~TextureManager();

private:

};

/**
 * @brief 
 * 
 */
enum class WrapParameter
{
    WRAP_S = GL_TEXTURE_WRAP_S,
    WRAP_T = GL_TEXTURE_WRAP_T,
    WRAP_R = GL_TEXTURE_WRAP_R
};

/**
 * @brief 
 * 
 */
enum class FilterParameter
{
    MIN_FILTER = GL_TEXTURE_MIN_FILTER,
    MAG_FILTER = GL_TEXTURE_MAG_FILTER
};

/**
 * @brief 
 * 
 */
enum class SwizzleParameter
{
    SWIZZLE_R = GL_TEXTURE_SWIZZLE_R,
    SWIZZLE_G = GL_TEXTURE_SWIZZLE_G,
    SWIZZLE_B = GL_TEXTURE_SWIZZLE_B,
    SWIZZLE_A = GL_TEXTURE_SWIZZLE_A,
    SWIZZLE_RGBA = GL_TEXTURE_SWIZZLE_RGBA
};

/**
 * @brief 
 * 
 */
enum class LodBiasParameter
{
    LOD_BIAS = GL_TEXTURE_LOD_BIAS
};

enum class WrapMode
{
    CLAMP = GL_CLAMP,
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
    REPEAT = GL_REPEAT
};

enum class FilterMode
{
    NEAREST = GL_NEAREST,
    NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    LINEAR = GL_LINEAR,
    LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
};

class Texture : public Object
{
public:
    typedef std::shared_ptr<Texture> Ptr;

    Texture();
    virtual ~Texture();

    void SetImage(Image::Ptr image);
    Image::Ptr GetImage() { return _image; }

    void SetWrap(WrapParameter paramter, WrapMode wrap);
    void SetFilter(FilterParameter parameter, FilterMode filter);
    void SetLodBias(LodBiasParameter parameter, float32 bias);

    int32 GetNumMipLevels() { return _image == nullptr ? 0 : _image->GetImageSource().mipLevels; }
    Internalformat GetInternalformat() { return _image == nullptr ? GL_NONE : _image->GetImageSource().internalFormat; }


protected:
    virtual void InitStorage() = 0;

protected:
    RenderResInstance _res;
    Image::Ptr _image;

    bool _immutable;
};

class Texture1D : public Texture
{
public:
    typedef std::shared_ptr<Texture1D> Ptr;

    Texture1D();
    virtual ~Texture1D();

protected:
    virtual void InitStorage() override;
};

class Texture2D : public Texture
{
public:
    typedef std::shared_ptr<Texture2D> Ptr;

    Texture2D();
    virtual ~Texture2D();

protected:
    virtual void InitStorage() override;

};

class Texture2DMultiSampler : public Texture
{
public:
    typedef std::shared_ptr<Texture2DMultiSampler> Ptr;

    Texture2DMultiSampler();
    virtual ~Texture2DMultiSampler();

protected:
    virtual void InitStorage() override;

};

class Texture3D : public Texture
{
public:
    typedef std::shared_ptr<Texture3D> Ptr;

    Texture3D();
    virtual ~Texture3D();

protected:
    virtual void InitStorage() override;
};

class TextureRectangle : public Texture
{
public:
    typedef std::shared_ptr<TextureRectangle> Ptr;

    TextureRectangle();
    virtual ~TextureRectangle();

protected:
    virtual void InitStorage() override;
};

class TextureBuffer : public Texture
{
public:
    typedef std::shared_ptr<TextureBuffer> Ptr;

    TextureBuffer();
    virtual ~TextureBuffer();

protected:
    virtual void InitStorage() override;
};

class TextureCube : public Texture
{
public:
    typedef std::shared_ptr<TextureCube> Ptr;

    TextureCube();
    virtual ~TextureCube();

protected:
    virtual void InitStorage() override;
};

class Texture1DArray : public Texture
{
public:
    typedef std::shared_ptr<Texture1DArray> Ptr;

    Texture1DArray();
    virtual ~Texture1DArray();

protected:
    virtual void InitStorage() override;
};

class Texture2DArray : public Texture
{
public:
    typedef std::shared_ptr<Texture2DArray> Ptr;

    Texture2DArray();
    virtual ~Texture2DArray();

protected:
    virtual void InitStorage() override;
};

class TextureCubeArray : public Texture
{
public:
    typedef std::shared_ptr<TextureCubeArray> Ptr;

    TextureCubeArray();
    virtual ~TextureCubeArray();

protected:
    virtual void InitStorage() override;
};

class Texture2DMultiSamplerArray : public Texture
{
public:
    typedef std::shared_ptr<Texture2DMultiSamplerArray> Ptr;

    Texture2DMultiSamplerArray();
    virtual ~Texture2DMultiSamplerArray();

protected:
    virtual void InitStorage() override;
};
} // namespace TwinkleGraphics

#endif