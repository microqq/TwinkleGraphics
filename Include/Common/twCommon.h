

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
#if defined(__linux__) || defined(__APPLE__)
    // #ifdef EXPORT_COMDLL
        #define __TWCOMExport __attribute__ ((__visibility__("default")))
    // #endif

    // #ifdef EXPORT_UIDLL
        #define __TWUIExport __attribute__ ((__visibility__("default")))
    // #endif

    // #ifdef EXPORT_UTILDLL
        #define __TWUTILExport __attribute__ ((__visibility__("default")))
    // #endif

    // #ifdef EXPORT_PLUGIN
        #define __TWPLUGINExport __attribute__ ((__visibility__("default")))
    // #endif

    // #ifdef EXPORT_EVENT
        #define __TWEVENTExport __attribute__ ((__visibility__("default")))
    // #endif

#elif defined _WIN32
    #ifdef EXPORT_COMDLL
        #define __TWCOMExport __declspec(dllexport)
    #else
        #define __TWCOMExport __declspec(dllimport)
    #endif

    #ifdef EXPORT_UIDLL
        #define __TWUIExport __declspec(dllexport)
    #else
        #define __TWUIExport __declspec(dllimport)
    #endif

    #ifdef EXPORT_UTILDLL
        #define __TWUTILExport __declspec(dllexport)
    #else
        #define __TWUTILExport __declspec(dllimport)
    #endif

    #ifdef EXPORT_PLUGIN
        #define __TWPLUGINExport __declspec(dllexport)
    #else
        #define __TWPLUGINExport __declspec(dllimport)
    #endif

    #ifdef EXPORT_EVENT
        #define __TWEVENTExport __declspec(dllexport)
    #else
        #define __TWEVENTExport __declspec(dllimport)
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

class IDestroyable
{
    public:
        virtual void Destroy() = 0;
};

extern const char *PlatformDefines[5];
extern std::vector<std::string> GLExtensions;

extern bool CheckSupportExtension(std::string exention);

} // namespace TwinkleGraphics


#endif