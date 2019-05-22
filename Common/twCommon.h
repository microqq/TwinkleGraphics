#ifndef TW_COMMON_H
#define TW_COMMON_H

#include <glm/glm.hpp>

#ifndef MAX_VIEWPORT_COUNT
    #define MAX_VIEWPORT_COUNT 8
#endif

namespace TwinkleGraphics
{
using namespace glm;

typedef ivec4 Rect;
typedef vec4 RGBA;

} // namespace TwinkleGraphics


#endif