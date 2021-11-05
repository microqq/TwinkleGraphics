
#ifndef TW_RENDERCONTEXT_H
#define TW_RENDERCONTEXT_H

#include <glew/glew.h>

#include "twCommon.h"
#include "twReference.h"


namespace TwinkleGraphics {

struct StateAttribute {};

/*------------------------------Draw Command--------------------------*/

struct DrawCommand {};

enum class PrimitiveType {
  POINTS = GL_POINTS,

  LINES = GL_LINES,
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

struct RenderState {};

struct RenderStateAttribute : public StateAttribute {};

/*------------------------------Raster State--------------------------*/

enum class CullMode {
  FRONT = GL_FRONT,
  BACK = GL_BACK,
  FRONT_BACK = GL_FRONT_AND_BACK
};
enum class PolygonMode { POINT = GL_POINT, LINE = GL_LINE, FILL = GL_FILL };
struct RasterState {};

struct RasterStateAttribute : public StateAttribute {};

/*------------------------------Per Fragment
 * Operation--------------------------*/

/**
 * @brief
 *
 */
struct PerFragmentOperation {};

enum class DepthFunc {};

enum class BlendFunc {};

/*------------------------------Vertex Attribute--------------------------*/

__TWCOMExport extern const char *VertexLayoutDefines[11];
__TWCOMExport extern const char *TextureLayoutDefines[11];

enum VertexLayoutFlag {
  DEFAULT = 0,
  HAS_COLOR = 1,
  HAS_BINORMAL = 2,
  HAS_NORMAL = 4,
  HAS_UV = 8,
  HAS_UV1 = 16,
  HAS_UV2 = 32,
  HAS_UV3 = 64,
  HAS_UV4 = 128,
  HAS_UV5 = 256,
  HAS_UV6 = 512,
  HAS_UV7 = 1024
};

struct VertexLayout {};

/*------------------------------Render Context--------------------------*/

/**
 * @brief Manage render resource & state.
 *
 */
class RenderContext {
public:
  RenderContext();
  ~RenderContext();
};

} // namespace TwinkleGraphics

#endif