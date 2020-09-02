

#ifndef TW_TEXTURE_H
#define TW_TEXTURE_H

#include "twCommon.h"

#include "twImage.h"
#include "twConsoleLog.h"

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
    TEXPARAMETER_ANISTROPIC_FILTER_MASK = 1 << 15,
    TEXPARAMETER_DEFAULT_MASK = TEXPARAMETER_WRAP_S_MASK | 
                                TEXPARAMETER_WRAP_T_MASK | 
                                TEXPARAMETER_FILTER_MIN_MASK |
                                TEXPARAMETER_FILTER_MAG_MASK |
                                TEXPARAMETER_BORDERCOLOR_MASK,
    TEXPARAMETER_DEFAULT_DIRTY_FLAG = TEXPARAMETER_FILTER_MIN_MASK | TEXPARAMETER_FILTER_MAG_MASK
};

typedef TexParameterMask TexParameterDirtyFlag;

struct TexParams
{
    WrapMode wrapModes[3];
    // WrapMode wrap_s;
    // WrapMode wrap_t;
    // WrapMode wrap_r;

    FilterMode filterModes[2];
    // FilterMode filter_min;
    // FilterMode filter_mag;

    SwizzleParam swizzleParameter;
    SwizzleMask swizzle[4];

    LodBiasParam lodParameter;
    float32 lodbias;

    MipMapBaseLevelParam baselevelParameter;
    int32 mipmapBaselevel = 0;
    MipMapMaxLevelParam maxlevelParameter;
    int32 mipmapMaxlevel = 1000;

    TextureBorderColorParam bordercolorParameter;
    vec4 borderColor;

    DepthStencilTextureMode depthstencilTexMode;
    
    DepthTextureCompareMode depthTexCompMode;
    DepthTextureCompareFunc depthTexCompFunc;

    TextureMinLODParam minLodParameter;
    int32 minLod = -1000;
    TextureMaxLODParam maxLodParameter;
    int32 maxLod = 1000;

    float32 anistropic;
    bool useMaxAnistropic = true;

    TexParams()
        : swizzleParameter(SwizzleParam::NONE)
        , lodParameter(LodBiasParam::NONE)
    {
        wrapModes[0] = wrapModes[1] = wrapModes[2] = WrapMode::NONE;
        filterModes[0] = filterModes[1] = FilterMode::NONE;
    }
};

typedef TexParams SamplerParams;

/**
 * @brief 
 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/
 */
class __TWExport Sampler : Object
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
    const RenderResourceHandle& GetRenderRes() { return _res; }

private:
    RenderResourceHandle _res;


};


/**
 * @brief 
 * "A GL texture object includes both categories. The first category represents dimensionality and other image parameters, 
 *  and the second category represents sampling state." ---- OpenGL 4.5(Core Profile)
 */
class __TWExport Texture : public Object
{
public:
    typedef std::shared_ptr<Texture> Ptr;

    Texture(bool immutable = true, bool genMipMap = false);
    virtual ~Texture();

    void CreateFromImage(Image::Ptr image)
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

    void Create(int32 width, int32 height, GLenum internalformat = GL_RGBA8
        , GLenum format = GL_RGBA, int32 miplevels = 1
        , int32 depth = -1, int32 slices = -1)
    {
        _width = width;
        _height = height;
        _internalformat = internalformat;
        _format = format;
        _miplevels = miplevels;
        _slices = slices;

        InitStorage();
    }

    void Bind() { glBindTexture(_res.type, _res.id); }
    void UnBind() { glBindTexture(_res.type, 0); }

    const RenderResourceHandle& GetTexResource() { return _res; }

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

    void SetAnistropic(float32 anistropic = -1.0f, bool maxAnistropic = true);

    Image::Ptr GetImage() { return _image; }
    const TexParams& GetTexParams() { return _parameters; }
    int32 GetNumMipLevels() 
    { 
        if(_image == nullptr)
        {
            return _miplevels;
        }
        else
        {
            return _image->GetImageSource().mipLevels; 
        }
    }
    InternalFormat GetInternalformat() 
    { 
        if(_image == nullptr)
        {
            return _internalformat;   
        }
        else
        {
            return _image->GetImageSource().internalFormat; 
        }
    }

    int32 GetWidth(int32 level = 0) 
    { 
        if(_image == nullptr)
        {
            return _width;
        }
        else
        {
            return _image->GetImageSource().mip[level].width;
        }
    }
    int32 GetHeight(int32 level = 0) 
    {
        if (_image == nullptr)
        {
            return _height;
        }
        else
        {
            return _image->GetImageSource().mip[level].height;
        }
    }
    int32 GetDepth(int32 level = 0) 
    { 
        if(_image == nullptr)
        {
            return -1;
        }
        else
        {
            return _image->GetImageSource().mip[level].depth;
        }
    }

    const RenderResourceHandle& GetRenderRes() { return _res; }

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
    virtual void ApplyAnistropicFilter();

protected:
    TexParams _parameters;
    RenderResourceHandle _res;
    Image::Ptr _image;
    Sampler::Ptr _sampler;
    TexParameterMask _mask;
    TexParameterDirtyFlag _dirtyFlag;

    GLenum _internalformat;
    GLenum _format;
    int32 _miplevels;

    int32 _width;
    int32 _height;
    int32 _depth;
    int32 _slices;

    bool _immutable;
    bool _generateMipMap;
};


struct TextureSlot
{
    // std::string texname;
    Texture::Ptr tex;
    int8 location;
    int8 slot;

    void Apply()
    {
        const RenderResourceHandle &res = tex->GetRenderRes();
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

    void UnBind()
    {
        const RenderResourceHandle &res = tex->GetRenderRes();
        glBindTexture(res.type, 0);
    }
};


class __TWExport Texture1D : public Texture
{
public:
    typedef std::shared_ptr<Texture1D> Ptr;

    Texture1D(bool immutable = true, bool genMipMap = false)
        : Texture(immutable, genMipMap)
    {
        _res.type = GL_TEXTURE_1D;

        Console::LogInfo("Texture: Create texture1D ", _res.id, "(hash: ", _res.hash, ").\n");
    }
    virtual ~Texture1D() {}

protected:
    virtual void InitStorage() override;
    virtual void ApplyWrapParameter() override
    {
        // wrap
        if (_parameters.wrapModes[0] != WrapMode::NONE)
        {
            glTexParameteri(_res.type, GL_TEXTURE_WRAP_S, (int32)(_parameters.wrapModes[0]));
        }
    }
};

class __TWExport Texture2D : public Texture
{
public:
    typedef std::shared_ptr<Texture2D> Ptr;

    Texture2D(bool immutable = true, bool genMipMap = false)
        : Texture(immutable, genMipMap)
    {
        _res.type = GL_TEXTURE_2D;

        Console::LogInfo("Texture: Create texture2D ", _res.id, " (hash: ", _res.hash, ").\n");
    }
    virtual ~Texture2D() {}

protected:
    virtual void InitStorage() override;
    virtual void ApplyWrapParameter() override
    {
        // wrap
        if (_parameters.wrapModes[0] != WrapMode::NONE)
        {
            glTexParameteri(_res.type, GL_TEXTURE_WRAP_S, (int32)(_parameters.wrapModes[0]));
        }
        if (_parameters.wrapModes[1] != WrapMode::NONE)
        {
            glTexParameteri(_res.type, GL_TEXTURE_WRAP_T, (int32)(_parameters.wrapModes[1]));
        }
    }
};

class __TWExport Texture2DMultiSample : public Texture
{
public:
    typedef std::shared_ptr<Texture2DMultiSample> Ptr;

    Texture2DMultiSample(uint32 samples, bool immutable = true)
            : Texture(immutable)
            , _samples(samples)
            , _fixedsampledlocation(true)
    {
        _res.type = GL_TEXTURE_2D_MULTISAMPLE;

        Console::LogInfo("Texture: Create texture2D-multisample ", _res.id, " (hash: ", _res.hash, ").\n");
    }

    virtual ~Texture2DMultiSample() {}

    uint32 GetSamples() { return _samples; }

protected:
    virtual void InitStorage() override;

    virtual void ApplyTexParameters() override
    {
        ApplySwizzleParameter();
        ApplyMipMapParameter();
        ApplyDepthStencilMode();
    }

private:
    uint32 _samples;
    bool _fixedsampledlocation;
};

class __TWExport Texture3D : public Texture
{
public:
    typedef std::shared_ptr<Texture3D> Ptr;

    Texture3D(bool immutable = true, bool genMipMap = false)
            : Texture(immutable, genMipMap)
    {
        _res.type = GL_TEXTURE_3D;

        Console::LogInfo("Texture: Create texture3D ", _res.id, " (hash: ", _res.hash, ").\n");
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
class __TWExport TextureRectangle : public Texture
{
public:
    typedef std::shared_ptr<TextureRectangle> Ptr;

    TextureRectangle(bool immutable = true)
            : Texture(immutable)
    {
        _res.type = GL_TEXTURE_RECTANGLE;

        Console::LogInfo("Texture: Create textureRectangle ", _res.id, " (hash: ", _res.hash, ").\n");
    }

    virtual ~TextureRectangle() {}

protected:
    virtual void InitStorage() override;

    virtual void ApplyWrapParameter() override
    {
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
class __TWExport TextureBuffer : public Texture
{
public:
    typedef std::shared_ptr<TextureBuffer> Ptr;

    TextureBuffer(bool immutable = true)
            : Texture(immutable)
    {
        _res.type = GL_TEXTURE_BUFFER;

        Console::LogInfo("Texture: Create textureBuffer ", _res.id, " (hash: ", _res.hash, ").\n");
    }

    virtual ~TextureBuffer();

    void SetBufferData(int32 size, int32 format, GLvoid* data)
    {
        if(size <= 0 || data == nullptr) return;

        _size = size;
        _internalformat = format;
        _bufferData = data;

        InitStorage();
    }

protected:
    virtual void InitStorage() override;

private:
    GLvoid* _bufferData;
    int32 _size;
    uint32 _buffer;
    int32 _internalformat;
};

class __TWExport TextureCube : public Texture
{
public:
    typedef std::shared_ptr<TextureCube> Ptr;

    TextureCube(bool immutable = true, bool genMipMap = false)
        : Texture(immutable, genMipMap)
    {
        _res.type = GL_TEXTURE_CUBE_MAP;

        Console::LogInfo("Texture: Create textureCube ", _res.id, " (hash: ", _res.hash, ").\n");
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
    Image::Ptr _imagePositiveX;
    Image::Ptr _imageNegativeX;
    Image::Ptr _imagePositiveY;
    Image::Ptr _imageNegativeY;
    Image::Ptr _imagePositiveZ;
    Image::Ptr _imageNegativeZ;
};

class __TWExport Texture1DArray : public Texture
{
public:
    typedef std::shared_ptr<Texture1DArray> Ptr;

    Texture1DArray(bool immutable = true, bool genMipMap = false)
            : Texture(immutable, genMipMap)
    {
        _res.type = GL_TEXTURE_1D_ARRAY;

        Console::LogInfo("Texture: Create texture1DArray ", _res.id, " (hash: ", _res.hash, ").\n");
    }

    virtual ~Texture1DArray() {}

protected:
    virtual void InitStorage() override;
};

class __TWExport Texture2DArray : public Texture
{
public:
    typedef std::shared_ptr<Texture2DArray> Ptr;

    Texture2DArray(bool immutable = true, bool genMipMap = false)
            : Texture(immutable, genMipMap)
    {
        _res.type = GL_TEXTURE_2D_ARRAY;

        Console::LogInfo("Texture: Create texture3DArray ", _res.id, " (hash: ", _res.hash, ").\n");
    }

    virtual ~Texture2DArray() {}

protected:
    virtual void InitStorage() override;
};

class __TWExport TextureCubeArray : public Texture
{
public:
    typedef std::shared_ptr<TextureCubeArray> Ptr;

    TextureCubeArray(bool immutable = true, bool genMipMap = false)
            : Texture(immutable, genMipMap)
    {
        _res.type = GL_TEXTURE_CUBE_MAP_ARRAY;

        Console::LogInfo("Texture: Create textureCubeArray ", _res.id, " (hash: ", _res.hash, ").\n");
    }

    virtual ~TextureCubeArray() {}

protected:
    virtual void InitStorage() override;
};

class __TWExport Texture2DMultiSampleArray : public Texture
{
public:
    typedef std::shared_ptr<Texture2DMultiSampleArray> Ptr;

    Texture2DMultiSampleArray(int32 samples, bool immutable = true)
            : Texture(immutable)
            , _samples(samples)
            , _fixedsampledlocation(true)
    {
        _res.type = GL_TEXTURE_2D_MULTISAMPLE_ARRAY;

        Console::LogInfo("Texture: Create texture2D-multisample-array ", _res.id, " (hash: ", _res.hash, ").\n");
    }

    virtual ~Texture2DMultiSampleArray() {}

    uint32 GetSamples() { return _samples; }

protected:
    virtual void InitStorage() override;

    virtual void ApplyTexParameters() override
    {
        ApplySwizzleParameter();
        ApplyMipMapParameter();
        ApplyDepthStencilMode();
    }

private:
    int32 _samples;
    bool _fixedsampledlocation;
};



class TextureManager;
typedef Singleton<TextureManager> TextureManagerInst;

class __TWExport TextureManager
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