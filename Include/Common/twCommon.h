

#ifndef TW_COMMON_H
#define TW_COMMON_H

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "twSingleton.h"
#include "twObject.h"

#ifndef MAX_VIEWPORT_COUNT
    #define MAX_VIEWPORT_COUNT 8
#endif

#ifndef SAFE_DEL
    #define SAFE_DEL(ptr) if((ptr) != nullptr) { delete (ptr); (ptr) = nullptr; }
#endif

#ifndef SAFE_DEL_ARR
    #define SAFE_DEL_ARR(ptr) if((ptr) != nullptr) { delete[] (ptr); (ptr) = nullptr; }
#endif

/**
 * @brief Refer to ogre plugin system. (https://ogrecave.github.io/ogre/)
 * 
 */
#if defined __linux__
  #define __TWExport __attribute__ ((__visibility__("default")))
#elif defined _WIN32
    // #define __TWExport __declspec(dllexport)
    #ifdef BUILDING_DLL
        #define __TWExport __declspec(dllexport)
        #define __SHADERExport __declspec(dllexport)
    #else
        #define __TWExport __declspec(dllimport)
        #define __SHADERExport __declspec(dllimport)
    #endif
#endif

namespace TwinkleGraphics
{
using namespace glm;

class Plugin;
class PluginManager;
typedef Plugin* (*INSTALL_PLUGIN_FUNC)(PluginManager*);
typedef void (*UNINSTALL_PLUGIN_FUNC)(PluginManager*);
typedef void (*IMGUI_FUNC)(void);

typedef void (*RENDER_ROUTINE)(void);

typedef ivec4 Rect;
typedef vec4 RGBA;

struct RenderResourceHandle
{
    union 
    {
        uint64 hash = 0;
        struct
        {
            uint32 id : 32;
            uint32 type : 32;
        };        
    };    
};

class IUpdatable
{
    public:
        virtual void Update() = 0;

    protected:
        IUpdatable() {}

    private:
        IUpdatable(const IUpdatable&) = delete;
        IUpdatable(IUpdatable&&) = delete;
        const IUpdatable& operator=(const IUpdatable&) = delete;
        const IUpdatable& operator=(IUpdatable&&) = delete;
};

extern const char *PlatformDefines[5];
extern std::vector<std::string> GLExtensions;

extern bool CheckSupportExtension(std::string exention);

} // namespace TwinkleGraphics


#endif