

#ifndef TW_TEXTURE_H
#define TW_TEXTURE_H

#include "twCommon.h"

#include "twImage.h"

namespace TwinkleGraphics
{
typedef GLenum InternalFormat;

enum class TextureType
{
    TEXTURE_1D = GL_TEXTURE_1D,
    TEXTURE_1D_ARRAY = GL_TEXTURE_1D_ARRAY,
    TEXTURE_2D = GL_TEXTURE_2D,
    TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
    TEXTURE_2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE,
    TEXTURE_2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
    TEXTURE_3D = GL_TEXTURE_3D,
    TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP,
    TEXTURE_CUBE_MAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY,
    TEXTURE_RECTANGLE = GL_TEXTURE_RECTANGLE,
    TEXTURE_BUFFER = GL_TEXTURE_BUFFER
};

/**
 * @brief 
 * 
 */
enum class WrapParam
{
    WRAP_S = GL_TEXTURE_WRAP_S,
    WRAP_T = GL_TEXTURE_WRAP_T,
    WRAP_R = GL_TEXTURE_WRAP_R
};

/**
 * @brief 
 * 
 */
enum class FilterParam
{
    MIN_FILTER = GL_TEXTURE_MIN_FILTER,
    MAG_FILTER = GL_TEXTURE_MAG_FILTER
};

/*------------------------------Sampler--------------------------*/

/**
 * @brief 
 * 
 */
enum class SwizzleParam
{
    NONE = GL_NONE,
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
enum class LodBiasParam
{
    NONE = GL_NONE,
    LOD_BIAS = GL_TEXTURE_LOD_BIAS
};

enum class WrapMode
{
    NONE = GL_NONE,
    CLAMP = GL_CLAMP,
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
    REPEAT = GL_REPEAT
};

enum class FilterMode
{
    NONE = GL_NONE,
    NEAREST = GL_NEAREST,
    NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    LINEAR = GL_LINEAR,
    LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
};

enum class SwizzleMask
{
    RED = GL_RED,
    GREEN = GL_GREEN,
    BLUE = GL_BLUE,
    ALPHA = GL_ALPHA,
    ZERO = GL_ZERO,
    ONE = GL_ONE
};

template <int32 L>
struct Swizzle;

template <>
struct Swizzle<1>
{
    SwizzleMask mask;
};

template <>
struct Swizzle<4>
{
    SwizzleMask mask[4];
};

typedef Swizzle<1> Swizzle1;
typedef Swizzle<4> Swizzle4;

struct TexParams
{
    WrapMode wrap_modes[3];
    // WrapMode wrap_s;
    // WrapMode wrap_t;
    // WrapMode wrap_r;

    FilterMode filter_modes[2];
    // FilterMode filter_min;
    // FilterMode filter_mag;

    SwizzleParam swizzle_parameter;
    SwizzleMask swizzle[4];

    LodBiasParam lod_parameter;
    float32 lodbias;

    TexParams()
        : swizzle_parameter(SwizzleParam::NONE)
        , lod_parameter(LodBiasParam::NONE)
    {
        wrap_modes[0] = wrap_modes[1] = wrap_modes[2] = WrapMode::NONE;
        filter_modes[0] = filter_modes[1] = FilterMode::NONE;
    //     swizzle_parameter = SwizzleParam::NONE;
    //     lod_parameter = LodBiasParam::NONE;
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
    void SetSwizzle(SwizzleParam parameter, SwizzleMask* masks);
    void SetLodBias(LodBiasParam parameter, float32 bias);

    Image::Ptr GetImage() { return _image; }
    const TexParams& GetTexParams() { return _parameters; }
    int32 GetNumMipLevels() { return _image == nullptr ? 0 : _image->GetImageSource().mipLevels; }
    InternalFormat GetInternalformat() { return _image == nullptr ? GL_NONE : _image->GetImageSource().internalFormat; }

    int32 GetWidth(int32 level = 0) { return _image->GetImageSource().mip[level].width; }
    int32 GetHeight(int32 level = 0) { return _image->GetImageSource().mip[level].height; }
    int32 GetDepth(int32 level = 0) { return _image->GetImageSource().mip[level].depth; }

    const RenderResInstance& GetRenderRes() { return _res; }

    virtual void ApplyTexParameters();

protected:
    virtual void InitStorage();
    virtual void ApplyWrapParameter();
    virtual void ApplyFilterParameter();
    virtual void ApplySwizzleParameter();
    virtual void ApplyLodBias();

protected:
    TexParams _parameters;
    RenderResInstance _res;
    Image::Ptr _image;

    bool _immutable;
    bool _initialized;
    bool _parameters_dirty;
};


struct TextureSlot
{
    // std::string texname;
    Texture::Ptr tex;
    int8 location;
    int8 slot;

    void Apply()
    {
        const RenderResInstance &res = tex->GetRenderRes();
        glBindTexture(res.type, res.id);
        tex->ApplyTexParameters();

        glActiveTexture(location);
    }
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
    virtual void ApplyWrapParameter() override
    {
        // wrap
        if (_parameters.wrap_modes[0] != WrapMode::NONE)
        {
            glTexParameteri(_res.type, GL_TEXTURE_WRAP_S, (int32)(_parameters.wrap_modes[0]));
        }
    }
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
    virtual void ApplyWrapParameter() override
    {
        // wrap
        if (_parameters.wrap_modes[0] != WrapMode::NONE)
        {
            glTexParameteri(_res.type, GL_TEXTURE_WRAP_S, (int32)(_parameters.wrap_modes[0]));
        }
        if (_parameters.wrap_modes[1] != WrapMode::NONE)
        {
            glTexParameteri(_res.type, GL_TEXTURE_WRAP_T, (int32)(_parameters.wrap_modes[1]));
        }
    }
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