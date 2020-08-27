

#ifndef TW_RESOURCE_H
#define TW_RESOURCE_H

#include <iostream>
#include <string>
#include <memory>
#include <map>

#include "twCommon.h"
#include "twRingBuffer.h"

namespace TwinkleGraphics
{
class ResourceReader;
class ResourceManager;
typedef Singleton<ResourceManager> ResourceManagerInst;
typedef uint64_t RederId;

enum class CacheHint
{
    NONE_CACHE = 0,
    CACHE_SOURCE = 1,
    CACHE_OBJECT = 2
};

class ReaderOption
{
public:
    ReaderOption()
        : _cacheHint(CacheHint::CACHE_SOURCE)
    {}
    ReaderOption(const ReaderOption& src)
    {
        _cacheHint = src._cacheHint;
    }
    ~ReaderOption()
    {}

    const ReaderOption& operator=(const ReaderOption& src)
    {
        _cacheHint = src._cacheHint;
        return *this;
    }

    inline void SetCacheHint(CacheHint hint) { _cacheHint = hint; }
    CacheHint GetCacheHint() { return _cacheHint; }

private:
    CacheHint _cacheHint = CacheHint::CACHE_SOURCE;
};


class ResourceReader
{
public:
    typedef std::shared_ptr<ResourceReader> Ptr;

    ResourceReader()
        : _option(nullptr)
    {}
    virtual ~ResourceReader() 
    {
        SAFE_DEL(_option);
    }

    void SetReaderOption(ReaderOption* option) 
    {
        if(option == nullptr)
        {
            return;
        } 

        if(_option == nullptr)
        {
            _option = new ReaderOption;
        }

        *_option = *option; 
    }
    const ReaderOption* const GetReaderOption() {  return this->_option; }

protected:
    ReaderOption* _option = nullptr;
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
        : _sharedObject(nullptr)
        , _status(status)
    {}
    ReadResult(TPtr shared_obj, Status status = Status::NONE)
        : _sharedObject(shared_obj)
        , _status(status)
    {}
    ReadResult(const ReadResult& copyop)
        : _sharedObject(copyop._sharedObject)
        , _status(copyop._status)
    {}
    ~ReadResult()
    {}

    TPtr GetSharedObject() const { return _sharedObject; }
    Status GetStatus() const { return _status; }
    ReadResult& operator =(const ReadResult& result) 
    {
        _sharedObject = result._sharedObject;
        _status = result._status;

        return *this;
    }

private:
    TPtr _sharedObject;
    Status _status;
};

class ResourceCache
{
public:
    typedef std::shared_ptr<ResourceCache> Ptr;

    ResourceCache()
    {
    }
    ~ResourceCache()
    {
    }

private:

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

        //Todo: if not found in cache, should get reader from pool. use placement new
        //else
        R* r = new R(std::forward<Args>(args)...);

        //  http://klamp.works/2015/10/09/call-template-method-of-template-class-from-template-function.html
        // error: use 'template' keyword to treat 'Read' as a dependent template name
        // return r->Read<TPtr>(filename, option);
        return r->template Read<TPtr>(filename, option);
    }

    template<class R, class TPtr, class... Args>
    ReadResult<TPtr> ReadAsync(const char* filename, ReaderOption* option, Args&&...args)
    {
        //get GUID with filename, read from cache

        //Todo: if not found in cache, should get reader from pool. use placement new
        //else
        R* r = new R(std::forward<Args>(args)...);

        //  http://klamp.works/2015/10/09/call-template-method-of-template-class-from-template-function.html
        // error: use 'template' keyword to treat 'Read' as a dependent template name
        // return r->Read<TPtr>(filename, option);
        return r->template Read<TPtr>(filename, option);
    }


private:
    typedef std::multimap<RederId, ResourceReader::Ptr> MultMapReaders;


};


} // namespace TwinkleGraphics

#endif