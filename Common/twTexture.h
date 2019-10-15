

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
    SWIZZLE_RGBA = GL_TEXTURE_SWIZZLE_RGBA,
    DEFAULT = SWIZZLE_RGBA
};

/**
 * @brief 
 * 
 */
enum class LodBiasParam
{
    NONE = GL_NONE,
    LOD_BIAS = GL_TEXTURE_LOD_BIAS,
    DEFAULT = LOD_BIAS
};

enum class WrapMode
{
    NONE = GL_NONE,
    CLAMP = GL_CLAMP,
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
    REPEAT = GL_REPEAT,
    DEFAULT = REPEAT
};

enum class FilterMode
{
    NONE = GL_NONE,
    NEAREST = GL_NEAREST,
    NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    LINEAR = GL_LINEAR,
    LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
    DEFAULT = LINEAR
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

enum class DepthStencilTextureMode
{
    NONE = GL_NONE,
    DEPTH = GL_DEPTH_COMPONENT,
    STENCIL = GL_STENCIL_INDEX
};

enum class DepthTextureCompareMode
{
    NONE = GL_NONE,
    COMPARE_REF_TO_TEXTURE = GL_COMPARE_REF_TO_TEXTURE
};

enum class DepthTextureCompareFunc
{
    NONE = GL_NONE,
    LEQUAL = GL_LEQUAL,
    GEQUAL = GL_GEQUAL,
    EQUAL = GL_EQUAL,
    NOTEQUAL = GL_NOTEQUAL,
    LESS = GL_LESS,
    GREATER = GL_GREATER,
    ALWAYS = GL_ALWAYS,
    NEVER = GL_NEVER
};

enum class TextureBorderColorParam
{
    NONE = GL_NONE,
    BORDER_COLOR = GL_TEXTURE_BORDER_COLOR
};

enum class MipMapBaseLevelParam
{
    NONE = GL_NONE,
    BESE_LEVEL = GL_TEXTURE_BASE_LEVEL
};

enum class MipMapMaxLevelParam
{
    NONE = GL_NONE,
    MAX_LEVEL = GL_TEXTURE_MAX_LEVEL
};

enum class TextureMinLODParam
{
    NONE = GL_NONE,
    MIN_LOD = GL_TEXTURE_MIN_LOD
};

enum class TextureMaxLODParam
{
    NONE = GL_NONE,
    MAX_LOD = GL_TEXTURE_MAX_LOD
};

enum TexParameterMask
{
    TEXPARAMETER_WRAP_S_MASK = 1 << 0,
    TEXPARAMETER_WRAP_T_MASK = 1 << 1,
    TEXPARAMETER_WRAP_R_MASK = 1 << 2,
    TEXPARAMETER_FILTER_MIN_MASK = 1 << 3,
    TEXPARAMETER_FILTER_MAG_MASK = 1 << 4,
    TEXPARAMETER_SWIZZLE_MASK = 1 << 5,
    TEXPARAMETER_LODBIAS_MASK = 1 << 6,
    TEXPARAMETER_BORDERCOLOR_MASK = 1 << 7,
    TEXPARAMETER_MIPMAP_BASE_LEVEL_MASK = 1 << 8,
    TEXPARAMETER_MIPMAP_MAX_LEVEL_MASK = 1 << 9,
    TEXPARAMETER_MIN_LOD_MASK = 1 << 10,
    TEXPARAMETER_MAX_LOD_MASK = 1 << 11,
    TEXPARAMETER_DEPTHSTENCIL_MASK = 1 << 12,
    TEXPARAMETER_DEPTH_TEX_COMP_MODE_MASK = 1 << 13,
    TEXPARAMETER_DEPTH_TEX_COMP_FUNC_MASK = 1 << 14,
    TEXPARAMETER_DEFAULT_MASK = TEXPARAMETER_WRAP_S_MASK | 
                                TEXPARAMETER_WRAP_T_MASK | 
                                TEXPARAMETER_FILTER_MIN_MASK |
                                TEXPARAMETER_FILTER_MAG_MASK |
                                TEXPARAMETER_BORDERCOLOR_MASK,
    TEXPARAMETER_DEFAULT_DIRTY_FLAG = 0
};

typedef TexParameterMask TexParameterDirtyFlag;

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

    MipMapBaseLevelParam baselevel_parameter;
    int32 mipmap_baselevel = 0;
    MipMapMaxLevelParam maxlevel_parameter;
    int32 mipmap_maxlevel = 1000;

    TextureBorderColorParam bordercolor_parameter;
    vec4 border_color;

    DepthStencilTextureMode depthstencil_tex_mode;
    
    DepthTextureCompareMode depth_tex_comp_mode;
    DepthTextureCompareFunc depth_tex_comp_func;

    TextureMinLODParam min_lod_parameter;
    int32 min_lod = -1000;
    TextureMaxLODParam max_lod_parameter;
    int32 max_lod = 1000;

    TexParams()
        : swizzle_parameter(SwizzleParam::NONE)
        , lod_parameter(LodBiasParam::NONE)
    {
        wrap_modes[0] = wrap_modes[1] = wrap_modes[2] = WrapMode::NONE;
        filter_modes[0] = filter_modes[1] = FilterMode::NONE;


    }
};

typedef TexParams SamplerParams;

/**
 * @brief 
 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/
 */
class Sampler : Object
{
public:
    typedef std::shared_ptr<Sampler> Ptr;

    Sampler()
        : Object()
    {
        uint32 samplers[1];
        glGenSamplers(1, samplers);

        _res.id = samplers[0];
    }
    virtual ~Sampler()
    {
        if(_res.id != 0)
        {
            uint32 samplers[1] = { _res.id };
            glDeleteSamplers(1, samplers);
        }
    }

    bool IsValid() { return _res.id != 0; }
    const RenderResInstance& GetRenderRes() { return _res; }

private:
    RenderResInstance _res;


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

    void SetImageData(const ImageData& src)
    {
        if(_image != nullptr) return;

        if(_data != nullptr)
        {
            SAFE_DEL(_data);
        }

        _data = new ImageData(src);
        InitStorage();
    }

    void SetSampler(Sampler::Ptr sampler) { _sampler = sampler; }
    Sampler::Ptr GetSampler() { return _sampler; }

    template<WrapParam Wrap>
    void SetWrap(WrapMode wrap);
    template<FilterParam Filter>
    void SetFilter(FilterMode filter);
    template<int32 L>
    void SetSwizzle(SwizzleParam parameter, Swizzle<L> swizzle);
    void SetSwizzle(SwizzleParam parameter, SwizzleMask* masks);
    void SetLodBias(LodBiasParam parameter, float32 bias);

    void SetMipMapBaseLevel(MipMapBaseLevelParam parameter, int32 level);
    void SetMipMapMaxLevel(MipMapMaxLevelParam parameter, int32 level);

    void SetTexBorderColor(TextureBorderColorParam parameter, const vec4& color);

    void SeTexMinLOD(TextureMinLODParam parameter, int32 lod);
    void SeTexMaxLOD(TextureMaxLODParam parameter, int32 lod);

    void SetDepthTexCompareMode(DepthTextureCompareMode mode);
    void SetDepthTexCompareFun(DepthTextureCompareFunc func);

    void SetDepthStencilMode(DepthStencilTextureMode mode);

    Image::Ptr GetImage() { return _image; }
    const TexParams& GetTexParams() { return _parameters; }
    int32 GetNumMipLevels() 
    { 
        return _image == nullptr ? (_data == nullptr ? 0 : _data->mipLevels)
            : _image->GetImageSource().mipLevels; 
    }
    InternalFormat GetInternalformat() 
    { 
        return _image == nullptr ? (_data == nullptr ? GL_NONE : _data->internalFormat)
        : _image->GetImageSource().internalFormat; 
    }

    int32 GetWidth(int32 level = 0) 
    { 
        return _image == nullptr ? (_data == nullptr ? 0 : _data->mip[level].width) 
            : _image->GetImageSource().mip[level].width; 
    }
    int32 GetHeight(int32 level = 0) 
    { 
        return _image == nullptr ? (_data == nullptr ? 0 : _data->mip[level].height) 
            : _image->GetImageSource().mip[level].height; 
    }
    int32 GetDepth(int32 level = 0) 
    { 
        return _image == nullptr ? (_data == nullptr ? 0 : _data->mip[level].depth) 
            : _image->GetImageSource().mip[level].depth; 
    }

    const RenderResInstance& GetRenderRes() { return _res; }

    void AppendTexParameterMask(TexParameterMask mask) { _mask = (TexParameterMask)(_mask | mask); }
    void SetTexParameterMask(TexParameterMask mask) { _mask = mask; }
    virtual void ApplyTexParameters();

protected:
    virtual void InitStorage();
    virtual void ApplyWrapParameter();
    virtual void ApplyFilterParameter();
    virtual void ApplySwizzleParameter();
    virtual void ApplyLodBias();
    virtual void ApplyDepthTexCompParameter();
    virtual void ApplyMipMapParameter();
    virtual void ApplyTexLODParameter();
    virtual void ApplyTexBorderColor();
    virtual void ApplyDepthStencilMode();

protected:
    TexParams _parameters;
    RenderResInstance _res;
    Image::Ptr _image;
    Sampler::Ptr _sampler;
    TexParameterMask _mask;
    TexParameterDirtyFlag _dirty_flag;

    ImageData* _data;

    bool _immutable;
    bool _initialized;
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

        Sampler::Ptr sampler = tex->GetSampler();
        if(sampler != nullptr && sampler->IsValid())
        {
            glBindSampler(location, sampler->GetRenderRes().id);
        }
        else
        {
            glBindSampler(location, 0);
        }

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

#ifdef _DEBUG
    std::cout << "Texture: Create texture1D " << _res.id << "(hash: " << _res.hash << ")." << std::endl;
#endif
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

#ifdef _DEBUG
    std::cout << "Texture: Create texture2D " << _res.id << " (hash: " << _res.hash << ")." << std::endl;
#endif
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

#ifdef _DEBUG
    std::cout << "Texture: Create texture2D-multisample " << _res.id << " (hash: " << _res.hash << ")." << std::endl;
#endif
    }

    virtual ~Texture2DMultiSample() {}

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

class Texture3D : public Texture
{
public:
    typedef std::shared_ptr<Texture3D> Ptr;

    Texture3D(bool immutable = true)
            : Texture(immutable)
    {
        _res.type = GL_TEXTURE_3D;

#ifdef _DEBUG
    std::cout << "Texture: Create texture3D " << _res.id << " (hash: " << _res.hash << ")." << std::endl;
#endif
    }

    virtual ~Texture3D() {}

protected:
    virtual void InitStorage() override;
};

/**
 * @brief 
 * https://stackoverflow.com/questions/25157306/gl-texture-2d-vs-gl-texture-rectangle
 * https://www.khronos.org/opengl/wiki/Rectangle_Texture
 */
class TextureRectangle : public Texture
{
public:
    typedef std::shared_ptr<TextureRectangle> Ptr;

    TextureRectangle(bool immutable = true)
            : Texture(immutable)
    {
        _res.type = GL_TEXTURE_RECTANGLE;

#ifdef _DEBUG
    std::cout << "Texture: Create textureRectangle " << _res.id << " (hash: " << _res.hash << ")." << std::endl;
#endif
    }

    virtual ~TextureRectangle() {}

protected:
    virtual void InitStorage() override;

    virtual void ApplyWrapParameter() override
    {
        // wrap only support clamp_to_edge/clamp_to_border
        // do not support mipmap
        // if (_parameters.wrap_modes[0] != WrapMode::NONE)
        // {
        //     glTexParameteri(_res.type, GL_TEXTURE_WRAP_S, (int32)(_parameters.wrap_modes[0]));
        // }
        // if (_parameters.wrap_modes[1] != WrapMode::NONE)
        // {
        //     glTexParameteri(_res.type, GL_TEXTURE_WRAP_T, (int32)(_parameters.wrap_modes[1]));
        // }
    }


    virtual void ApplyFilterParameter() override
    {
        // filter only support nearest/linear

    }
};


/**
 * @brief 
 * https://stackoverflow.com/questions/21424968/what-is-the-purpose-of-opengl-texture-buffer-objects
 * https://stackoverflow.com/questions/6281109/texture-buffer-objects-or-regular-textures?rq=1
 */
class TextureBuffer : public Texture
{
public:
    typedef std::shared_ptr<TextureBuffer> Ptr;

    TextureBuffer(bool immutable = true)
            : Texture(immutable)
    {
        _res.type = GL_TEXTURE_BUFFER;

#ifdef _DEBUG
    std::cout << "Texture: Create textureBuffer " << _res.id << " (hash: " << _res.hash << ")." << std::endl;
#endif
    }

    virtual ~TextureBuffer();

    void SetBufferData(int32 size, int32 format, GLvoid* data)
    {
        if(size <= 0 || data == nullptr) return;

        _size = size;
        _internalformat = format;
        _buffer_data = data;

        InitStorage();
    }

protected:
    virtual void InitStorage() override;

private:
    GLvoid* _buffer_data;
    int32 _size;
    uint32 _buffer;
    int32 _internalformat;
};

class TextureCube : public Texture
{
public:
    typedef std::shared_ptr<TextureCube> Ptr;

    TextureCube(bool immutable = true)
        : Texture(immutable)
    {
        _res.type = GL_TEXTURE_CUBE_MAP;

#ifdef _DEBUG
    std::cout << "Texture: Create textureCube " << _res.id << " (hash: " << _res.hash << ")." << std::endl;
#endif
    }
    virtual ~TextureCube() {}

    void SetPositiveX(Image::Ptr image);
    void SetPositiveY(Image::Ptr image);
    void SetPositiveZ(Image::Ptr image);

    void SetNegativeX(Image::Ptr image);
    void SetNegativeY(Image::Ptr image);
    void SetNegativeZ(Image::Ptr image);

    void InitStorageByOthers() { InitStorage(); }

protected:
    virtual void InitStorage() override;

private:
    void InitTexStorage(int32 target, ImageData *data);
    void InitTexStorage(ImageData *data);

private:
    Image::Ptr _image_positive_x;
    Image::Ptr _image_negative_x;
    Image::Ptr _image_positive_y;
    Image::Ptr _image_negative_y;
    Image::Ptr _image_positive_z;
    Image::Ptr _image_negative_z;
};

class Texture1DArray : public Texture
{
public:
    typedef std::shared_ptr<Texture1DArray> Ptr;

    Texture1DArray(bool immutable = true)
            : Texture(immutable)
    {
        _res.type = GL_TEXTURE_1D_ARRAY;

#ifdef _DEBUG
    std::cout << "Texture: Create texture1DArray " << _res.id << " (hash: " << _res.hash << ")." << std::endl;
#endif
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

#ifdef _DEBUG
    std::cout << "Texture: Create texture3DArray " << _res.id << " (hash: " << _res.hash << ")." << std::endl;
#endif
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

#ifdef _DEBUG
    std::cout << "Texture: Create textureCubeArray " << _res.id << " (hash: " << _res.hash << ")." << std::endl;
#endif
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

#ifdef _DEBUG
    std::cout << "Texture: Create texture2D-multisample-array " << _res.id << " (hash: " << _res.hash << ")." << std::endl;
#endif
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