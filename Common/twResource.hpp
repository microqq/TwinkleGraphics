

#ifndef TW_RESOURCE_H
#define TW_RESOURCE_H

#include <iostream>
#include <string>
#include <map>

#include "twCommon.h"

namespace TwinkleGraphics
{
class Resource;
class ResourceLoader;
class ResourceManager;
typedef Singleton<ResourceManager> ResourceManagerInst;

class ResourceManager
{
public:
    ResourceManager() {}
    ~ResourceManager() {}

    void RegisterLoader(const char* type, ResourceLoader* loader)
    {
        MapLoaders::iterator it = _loaders.find(type);
        if(it != _loaders.end())
        {
            std::cout << "Error: Loader " << type << "already exist." << std::endl; 
            return;
        }

        _loaders.insert(MapLoaders::value_type(type, loader));
    }
    void UnRegisterLoader(const char* type)
    {
        if(FindResourceLoader(type) != nullptr)
        {
            _loaders.erase(type);
        }
    }
    void UnRegisterLoader(ResourceLoader* loader)
    {
        ResourceLoader* find = FindResourceLoader(loader);
        if(find == nullptr)
        {
            std::cout << "Error: Loader " << loader->GetType() << "already unregistered." << std::endl; 
            return;
        }

        _loaders.erase(loader->GetType());
    }
    ResourceLoader* FindResourceLoader(const char* type) 
    {
        MapLoaders::iterator it = _loaders.find(type);
        if(it != _loaders.end())
        {
            return it->second;
        }

        std::cout << "Warning: can not find Loader " << type << "." << std::endl; 
        return nullptr;
    }
    ResourceLoader* FindResourceLoader(ResourceLoader* loader)
    {
        for(auto l : _loaders)
        {
            if(l.second == loader)
            {
                return l.second;
            }
        }

        std::cout << "Warning: can not find Loader " << loader->GetType() << "." << std::endl; 
        return nullptr;
    }

private:
    typedef std::map<std::string, ResourceLoader*> MapLoaders;

    std::map<std::string, ResourceLoader*> _loaders;
};

class Resource : public Object
{
public:
    Resource()
        : Object()
    {}
    virtual ~Resource() {}

protected:
    uint32 _hash;
};

class ResourceLoader
{
public:
    ResourceLoader(const char* type)
        : _type(type)
    {
        ResourceManagerInst resMgr;
        resMgr->RegisterLoader(type, this);
    }
    virtual ~ResourceLoader()
    {
        ResourceManagerInst resMgr;
        resMgr->UnRegisterLoader(_type.c_str());
    }

    const char* GetType() { return _type.c_str(); }

private:
    std::string _type;
};

} // namespace TwinkleGraphics

#endif