
#ifndef TW_RENDERCONTEXT_H
#define TW_RENDERCONTEXT_H

#include <glew/glew.h>

#include "twReference.hpp"
#include "twCommon.h"

namespace TwinkleGraphics
{

struct StateAttribute
{

};

/*------------------------------Draw Command--------------------------*/

struct DrawCommand
{

};

enum class PrimitiveType
{
    POINTS = GL_POINTS,

    LINES =  GL_LINES,
    LINE_STRIP = GL_LINE_STRIP,
    LINES_ADJACENCY = GL_LINES_ADJACENCY,
    LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,

    TRIANGLES = GL_TRIANGLES,
    TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
    TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
    TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,

    QUADS = GL_QUADS,
    QUAD_STRIP = GL_QUAD_STRIP
};


/*------------------------------Render State--------------------------*/

struct RenderState
{

};


struct RenderStateAttribute : public StateAttribute
{

};

/*------------------------------Raster State--------------------------*/

enum class CullMode
{
    NONE = GL_NONE,
};
enum class PolygonMode
{
    
};
struct RasterState
{

};

struct RasterStateAttribute : public StateAttribute
{
    
};



/*------------------------------Per Fragment Operation--------------------------*/

/**
 * @brief 
 * 
 */
struct PerFragmentOperation
{

};

enum class DepthFunc
{};

enum class BlendFunc
{};


/*------------------------------Vertex Attribute--------------------------*/

struct VertexLayout
{

};

struct VertexLayoutAttrib
{

};





/*------------------------------Render Context--------------------------*/

/**
 * @brief Manage render resource & state.
 * 
 */
class RenderContext
{
public:
    RenderContext();
    ~RenderContext();

    void CreateVertexBuffer();

};


} // namespace TwinkleGraphics


#endif