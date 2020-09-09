/**
 * @brief Refer to ogre plugin system. (https://ogrecave.github.io/ogre/)
 * 
 */

#include <iostream>

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

DynLib *DynLibManager::GetDynLib(std::string &path)
{
    using MapDynLib = std::map<std::string, DynLib*>;
    MapDynLib::iterator it = _dynlibs.find(path);
    if(it != _dynlibs.end())
    {
        return it->second;
    }

    return nullptr;
}

DynLib* DynLibManager::Load(std::string &path)
{
    using MapDynLib = std::map<std::string, DynLib*>;
    MapDynLib::iterator it = _dynlibs.find(path);
    if(it != _dynlibs.end())
    {
        return  nullptr;
    }

    DynLib* dynlib = new DynLib(path);
    dynlib->Load();
    if(!(dynlib->IsLoaded()))
    {
        SAFE_DEL(dynlib);
        return nullptr;
    }

    _dynlibs.insert(MapDynLib::value_type(path, dynlib));
    return dynlib;
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
    _handle = nullptr;
}

void DynLib::Load()
{
    if(_loaded)
    {
        _unloaded = false;
        return;
    }

    bool name_correct = false;
#if defined(__linux__) || defined(__APPLE__)
    name_correct = _name.find(".so") != std::string::npos;
#elif _WIN32
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

    bool success = !(DYNLIB_UNLOAD(_handle));
#ifdef __linux__  ||defined(__APPLE__)
#elif _WIN32
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

void* DynLib::GetSymbol(std::string& symbol)
{
    return (void*)DYNLIB_GETSYM(_handle, symbol.c_str());
}

} // namespace TwinkleGraphics
