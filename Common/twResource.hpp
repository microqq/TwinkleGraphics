

#ifndef TW_RESOURCE_H
#define TW_RESOURCE_H

#include <iostream>
#include <string>
#include <map>
#include <memory>

#include "twCommon.h"

namespace TwinkleGraphics
{
class Resource;
class ResourceReader;
class ResourceManager;
typedef Singleton<ResourceManager> ResourceManagerInst;

class Resource : public Object
{
public:
    typedef std::shared_ptr<Resource> Ptr;
    typedef std::weak_ptr<Resource> WeakPtr;

    Resource()
        : Object()
    {}
    virtual ~Resource() {}

protected:
    uint32 _hash;
};


class ReadResult
{
public:
    enum class Status
    {
        SUCCESS,
        FAILED
    };

    ReadResult(Status status)
    {}
    ReadResult(Object* obj, Status status)
    {}
    ReadResult(Object::Ptr obj, Status status)
    {}
    ~ReadResult();

private:
    Object::Ptr _shared_object;
    Object* _object;
    Status _status;
};

class ResourceReader
{
public:
    ResourceReader(const char* type);
    virtual ~ResourceReader();

    const char* GetType() { return _type.c_str(); }
    // ReadResult Read();

private:
    std::string _type;
};

class ResourceManager
{
public:
    ResourceManager() {}
    ~ResourceManager() {}

    void RegisterReader(const char* type, ResourceReader* reader)
    {
        MapLoaders::iterator it = _readers.find(type);
        if(it != _readers.end())
        {
            std::cout << "Error: Loader " << type << "already exist." << std::endl; 
            return;
        }

        _readers.insert(MapLoaders::value_type(type, reader));
    }
    void UnRegisterReader(const char* type)
    {
        ResourceReader* reader = FindResourceReader(type);
        if(reader != nullptr)
        {
            SAFE_DEL(reader);
            _readers.erase(type);
        }
    }
    void UnRegisterReader(ResourceReader* reader)
    {
        ResourceReader* find = FindResourceReader(reader);
        if(find == nullptr)
        {
            std::cout << "Error: Loader " << reader->GetType() << "already unregistered." << std::endl; 
            return;
        }

        SAFE_DEL(reader);
        _readers.erase(reader->GetType());
    }
    ResourceReader* FindResourceReader(const char* type) 
    {
        MapLoaders::iterator it = _readers.find(type);
        if(it != _readers.end())
        {
            return it->second;
        }

        std::cout << "Warning: can not find Loader " << type << "." << std::endl; 
        return nullptr;
    }
    ResourceReader* FindResourceReader(ResourceReader* reader)
    {
        for(auto l : _readers)
        {
            if(l.second == reader)
            {
                return l.second;
            }
        }

        std::cout << "Warning: can not find Loader " << reader->GetType() << "." << std::endl; 
        return nullptr;
    }



private:
    typedef std::map<std::string, ResourceReader*> MapLoaders;

    std::map<std::string, ResourceReader*> _readers;
};


} // namespace TwinkleGraphics

#endif