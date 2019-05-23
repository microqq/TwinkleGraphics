

#ifndef TW_COMMON_H
#define TW_COMMON_H

#include <glm/glm.hpp>
#include "twSingleton.hpp"

#ifndef MAX_VIEWPORT_COUNT
    #define MAX_VIEWPORT_COUNT 8
#endif

#ifndef SAFE_DEL
    #define SAFE_DEL(ptr) if((ptr) != nullptr) { delete (ptr); (ptr) = nullptr; }
#endif

#ifndef SAFE_DEL_ARR
    #define SAFE_DEL_ARR(ptr) if((ptr) != nullptr) { delete[] (ptr); (ptr) = nullptr; }
#endif

namespace TwinkleGraphics
{
using namespace glm;

typedef ivec4 Rect;
typedef vec4 RGBA;

} // namespace TwinkleGraphics


#endif