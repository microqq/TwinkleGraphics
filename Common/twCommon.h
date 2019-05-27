

#ifndef TW_COMMON_H
#define TW_COMMON_H

#include <glm/glm.hpp>
#include "twSingleton.hpp"
#include "twObject.hpp"

#ifndef MAX_VIEWPORT_COUNT
    #define MAX_VIEWPORT_COUNT 8
#endif

#ifndef SAFE_DEL
    #define SAFE_DEL(ptr) if((ptr) != nullptr) { delete (ptr); (ptr) = nullptr; }
#endif

#ifndef SAFE_DEL_ARR
    #define SAFE_DEL_ARR(ptr) if((ptr) != nullptr) { delete[] (ptr); (ptr) = nullptr; }
#endif

#if defined __linux__
  #define __TWExport __attribute__ ((__visibility__("default")))
#elif defined _WIN32 || _WIN64
    #define __TWExport __declspec(dllexport)
//   #ifdef BUILDING_DLL
//     #define __TWExport __declspec(dllexport)
//   #else
//     #define __TWExport __declspec(dllimport)
#endif

namespace TwinkleGraphics
{
using namespace glm;

class Plugin;
typedef Plugin* (*INSTALL_PLUGIN_FUNC)(void);
typedef void (*UNINSTALL_PLUGIN_FUNC)(void);

typedef void (*RENDER_ROUTINE)(void);

typedef ivec4 Rect;
typedef vec4 RGBA;

struct RenderResType
{
    enum Type
    {
        
    };
};

struct RenderResInstance
{
    union 
    {
        uint64 hash;
        struct
        {
            uint32 id : 32;
            uint32 type : 32;
        };        
    };    
};

} // namespace TwinkleGraphics


#endif