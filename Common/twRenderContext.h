
#ifndef TW_RENDERMANAGER_H
#define TW_RENDERMANAGER_H

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


struct RasterState
{

};

struct FragmentOperation
{

};

enum class CullMode
{

};

enum class PolygonMode
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



struct VertexLayout
{

};

struct VertexLayoutAttrib
{

};


} // namespace TwinkleGraphics


#endif