

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
        NONE,
        SUCCESS,
        FAILED
    };

    ReadResult(Status status = Status::NONE)
    {}
    ReadResult(Object *obj, Status status = Status::NONE)
    {}
    ReadResult(Object::Ptr obj, Status status = Status::NONE)
    {}
    ~ReadResult()
    {}

private:
    Object::Ptr _shared_object;
    Object* _object;
    Status _status;
};

class ReaderOption
{
public:
    ReaderOption()
    {}
    virtual ~ReaderOption()
    {}
};

class ResourceReader
{
public:
    ResourceReader();
    virtual ~ResourceReader();

    virtual ReadResult Read(const char *filename, ReaderOption *option) 
    {
        return ReadResult();
    }

};

class ResourceManager
{
public:
    ResourceManager() {}
    ~ResourceManager() {}

    template<class R, class... Args>
    ReadResult Read(const char* filename, ReaderOption* option, Args&&...args)
    {
        //should get reader from pool. use placement new
        R* r = new R(std::forward<Args>(args)...);

        return r->Read(filename, option);
    }

private:
    typedef std::map<std::string, ResourceReader*> MapLoaders;

    std::map<std::string, ResourceReader*> _readers;
};


} // namespace TwinkleGraphics

#endif