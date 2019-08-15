
#ifndef TW_RENDERCONTEXT_H
#define TW_RENDERCONTEXT_H

#include <glew/glew.h>
#include "twCommon.h"

namespace TwinkleGraphics
{
struct DrawCommand
{

};

struct RenderState
{

};


/*------------------------------Raster State--------------------------*/

enum class CullMode
{

};
enum class PolygonMode
{
    
};
struct RasterState
{

};



/*------------------------------Per Fragment Operation--------------------------*/

struct PerFragmentOperation
{

};

enum class DepthFunc
{};

enum class BlendFunc
{};

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

template<int32 L>
struct Swizzle;

template<>
struct Swizzle<1>
{
    SwizzleMask mask;
};

template<>
struct Swizzle<4>
{
    SwizzleMask mask[4];
};




/*------------------------------Vertex Attribute--------------------------*/

struct VertexLayout
{

};

struct VertexLayoutAttrib
{

};



/*------------------------------Uniform--------------------------*/

enum UniformType
{
    UNI_FLOAT,
    UNI_DOUBLE,
    UNI_UINT,
    UNI_INT,
    UNI_BOOL,
    UNI_FVEC1,
    UNI_FVEC2,
    UNI_FVEC3,
    UNI_FVEC4,
    UNI_DVEC1,
    UNI_DVEC2,
    UNI_DVEC3,
    UNI_DVEC4,
    UNI_IVEC1,
    UNI_IVEC2,
    UNI_IVEC3,
    UNI_IVEC4,
    UNI_UIVEC1,
    UNI_UIVEC2,
    UNI_UIVEC3,
    UNI_UIVEC4,
    UNI_FMAT2,
    UNI_FMAT23,
    UNI_FMAT24,
    UNI_FMAT3,
    UNI_FMAT32,
    UNI_FMAT34,
    UNI_FMAT4,
    UNI_FMAT42,
    UNI_FMAT43,
    UNI_DMAT2,
    UNI_DMAT23,
    UNI_DMAT24,
    UNI_DMAT3,
    UNI_DMAT32,
    UNI_DMAT34,
    UNI_DMAT4,
    UNI_DMAT42,
    UNI_DMAT43
};

struct Uniform
{
    UniformType uni_type;
    std::string name;

    Uniform(UniformType unitype, const char* nchars)
        : uni_type(unitype)
        , name(nchars)
    {}
};


template<class T, uint32 N>
struct SimpleUniform;

template<class T>
struct SimpleUniform<T, 1> : public Uniform
{
    T u0;

    SimpleUniform(UniformType unitype, const char* nchars)
        : Uniform(unitype, nchars)
    {}

    void Set(T v) { u0 = v; }
    void BindLocation(uint32 location);
};
template<class T>
struct SimpleUniform<T, 2> : public Uniform
{
    T u0, u1;

    SimpleUniform(UniformType unitype, const char* nchars)
        : Uniform(unitype, nchars)
    {}

    void Set(T v0, T v1) { u0 = v0; u0 = v1; }
    void BindLocation(uint32 location);
};
template<class T>
struct SimpleUniform<T, 3> : public Uniform
{
    T u0, u1, u2;

    SimpleUniform(UniformType unitype, const char* nchars)
        : Uniform(unitype, nchars)
    {}

    void Set(T v0, T v1, T v2) { u0 = v0; u0 = v1; u2 = v2; }
    void BindLocation(uint32 location);
};
template<class T>
struct SimpleUniform<T, 4> : public Uniform
{
    T u0, u1, u2, u3;

    SimpleUniform(UniformType unitype, const char* nchars)
        : Uniform(unitype, nchars)
    {}

    void Set(T v0, T v1, T v2, T v3) { u0 = v0; u0 = v1; u2 = v2, u3 = v3; }
    void BindLocation(uint32 location);
};




template<class T, uint32 N>
struct VecUniform : public Uniform
{
    vec<N, T, defaultp> vector;

    VecUniform(UniformType unitype, const char* nchars)
        : Uniform(unitype, nchars)
    {}


    void Set(vec<N, T, defaultp> vec) { vector = vec; }
    void BindLocation(uint32 location);
};




template<class T, uint32 Row, uint32 Column>
struct MatUniform : public Uniform
{
    mat<Row, Column, T, defaultp> matrix;

    MatUniform(UniformType unitype, const char* nchars)
        : Uniform(unitype, nchars)
    {}

    void Set(mat<Row, Column, T, defaultp> mat) { matrix = mat; }
    void BindLocation(uint32 location, bool transpose);
};


class UniformBind
{
public:
    UniformBind();
    ~UniformBind();

};

} // namespace TwinkleGraphics


#endif