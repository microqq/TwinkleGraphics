

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


/**
 * @brief 
 * 
 * @tparam TPtr 
 */
template<class TPtr>
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
        : _shared_object(nullptr)
        , _status(status)
    {}
    ReadResult(TPtr shared_obj, Status status = Status::NONE)
        : _shared_object(shared_obj)
        , _status(status)
    {}
    ReadResult(const ReadResult& copyop)
        : _shared_object(copyop._shared_object)
        , _status(copyop._status)
    {}
    ~ReadResult()
    {}

    TPtr GetSharedObject() const { return _shared_object; }
    Status GetStatus() const { return _status; }
    ReadResult& operator =(const ReadResult& result) 
    {
        _shared_object = result._shared_object;
        _status = result._status;

        return *this;
    }

private:
    TPtr _shared_object;
    Status _status;
};

class ReaderOption
{
public:
    ReaderOption()
    {}
    ~ReaderOption()
    {}
};

enum class ResourceCacheHint
{      
};
class ResourceCache
{
};

class ResourceManager
{
public:
    ResourceManager() {}
    ~ResourceManager() {}

    /**
     * @brief 
     * 
     * @tparam R 
     * @tparam TPtr 
     * @tparam Args 
     * @param filename 
     * @param option 
     * @param args 
     * @return ReadResult<TPtr> 
     */
    template<class R, class TPtr, class... Args>
    ReadResult<TPtr> Read(const char* filename, ReaderOption* option, Args&&...args)
    {
        //get GUID with filename, read from cache


        //if not found in cache
        //should get reader from pool. use placement new
        R* r = new R(std::forward<Args>(args)...);

        //  http://klamp.works/2015/10/09/call-template-method-of-template-class-from-template-function.html
        // error: use 'template' keyword to treat 'Read' as a dependent template name
        // return r->Read<TPtr>(filename, option);
        return r->template Read<TPtr>(filename, option);
    }

private:
    typedef std::map<std::string, ResourceReader*> MapLoaders;

    std::map<std::string, ResourceReader*> _readers;
};


} // namespace TwinkleGraphics

#endif