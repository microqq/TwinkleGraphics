
/**
 * @brief Refer to ogre plugin system. (https://ogrecave.github.io/ogre/)
 * 
 */

#ifndef TW_DYNLIB_H
#define TW_DYNLIB_H

#include <string>
#include <map>

#ifdef __linux__
#include <dlfcn.h>
#elif _WIN32 || _WIN64
#include <windows.h>
#endif

#include "twCommon.h"

#ifdef __linux__
#    define DYNLIB_HANDLE void*
#    define DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY | RTLD_GLOBAL)
#    define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#    define DYNLIB_UNLOAD( a ) dlclose( a )
#elif _WIN32 || _WIN64
#    define DYNLIB_HANDLE hInstance
#    define DYNLIB_LOAD( a ) LoadLibraryEx( a, NULL, 0 ) // we can not use LOAD_WITH_ALTERED_SEARCH_PATH with relative paths
#    define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define DYNLIB_UNLOAD( a ) !FreeLibrary( a )
#endif

namespace TwinkleGraphics
{

class DynLib;
class DynLibManager;
typedef Singleton<DynLibManager> DynLibManagerInst;

class DynLibManager
{
public:
    DynLibManager();
    ~DynLibManager();

    void Load(std::string& path);
    void Unload(std::string& path);

private:
    std::map<std::string, DynLib*> _dynlibs;
};

class DynLib
{
public:
    DynLib(std::string& name);
    ~DynLib();

    void Load();
    void Unload();

    inline const std::string& GetName() { return _name; }
    inline bool IsLoaded() { return _loaded; }
    inline bool IsUnloaded() { return _unloaded; }

private:
    std::string _name;
    DYNLIB_HANDLE _handle;

    bool _loaded;
    bool _unloaded;
};
} // namespace TwinkleGraphics

#endif