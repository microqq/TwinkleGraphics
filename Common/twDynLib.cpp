/**
 * @brief Refer to ogre plugin system. (https://ogrecave.github.io/ogre/)
 * 
 */

#include <filesystem>

#include "twDynLib.h"

namespace TwinkleGraphics
{

DynLibManager::DynLibManager()
{
}

DynLibManager::~DynLibManager()
{
    _dynlibs.clear();
}

void DynLibManager::Load(std::string &path)
{
    using MapDynLib = std::map<std::string, DynLib*>;
    MapDynLib::iterator it = _dynlibs.find(path);
    if(it != _dynlibs.end())
    {
        return;
    }

    DynLib* dynlib = new DynLib(path);
    dynlib->Load();

    _dynlibs.insert(MapDynLib::value_type(path, dynlib));
}

void DynLibManager::Unload(std::string &path)
{
    using MapDynLib = std::map<std::string, DynLib*>;
    MapDynLib::iterator it = _dynlibs.find(path);
    if(it == _dynlibs.end())
    {
        return;
    }

    DynLib* dynlib = it->second;
    dynlib->Unload();

    SAFE_DEL(dynlib);

    _dynlibs.erase(it->first);

    return;
}



DynLib::DynLib(std::string &name)
    : _name(name)
    , _handle(nullptr)
    , _loaded(false)
    , _unloaded(false)
{
}

DynLib::~DynLib()
{
}

void DynLib::Load()
{
    if(_loaded)
    {
        _unloaded = false;
        return;
    }

    bool name_correct = false;
#ifdef __linux__
    name_correct = _name.find(".so") != std::string::npos;
#elif _WIN32 || _WIN64
    name_correct = _name.substr(_name.find_last_of(".") + 1) == "dll";
#endif

    if(!name_correct)
    {
#ifdef _DEBUG
    //assert
#endif
        return;
    }

    _handle = DYNLIB_LOAD(_name.c_str());
    _loaded = _handle != nullptr;
}

void DynLib::Unload()
{
    if(_unloaded)
    {
        _loaded = false;
        return;
    }

    bool success = DYNLIB_UNLOAD(_handle);
#ifdef __linux__  
#elif _WIN32 || _WIN64
#endif    

    if(!success)
    {
#ifdef _DEBUG
    //assert
#endif

        return;
    }

    _unloaded = true;
}
} // namespace TwinkleGraphics
