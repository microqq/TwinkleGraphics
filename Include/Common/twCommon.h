

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
    #ifdef EXPORT_DLL
        #define __TWExport __declspec(dllexport)
    #else
        #define __TWExport __declspec(dllimport)
    #endif

    #ifdef EXPORT_SINGLETON
        #define __SINGLETONExport __declspec(dllexport)
    #else
        #define __SINGLETONExport __declspec(dllimport)
    #endif

    #ifdef EXPORT_EVENT
        #define __EVENTExport __declspec(dllexport)
    #else
        #define __EVENTExport __declspec(dllimport)
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
        virtual ~IUpdatable() {}
};

class INonCopyable
{
    private:
        INonCopyable(const INonCopyable&) = delete;
        INonCopyable(INonCopyable&&) = delete;
        INonCopyable& operator=(const INonCopyable&) = delete;
        INonCopyable& operator=(INonCopyable&&) = delete;

    protected:
        INonCopyable() {}
        virtual ~INonCopyable() {}
};

extern const char *PlatformDefines[5];
extern std::vector<std::string> GLExtensions;

extern bool CheckSupportExtension(std::string exention);

} // namespace TwinkleGraphics


#endif