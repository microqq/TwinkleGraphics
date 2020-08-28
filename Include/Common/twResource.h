

#ifndef TW_RESOURCE_H
#define TW_RESOURCE_H

#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <tuple>

#include "twCommon.h"
#include "twRingBuffer.h"
#include "twThreadPool.h"

#define DECLARE_READERID public: static ReaderId ID
#define DEFINE_READERID(T) ReaderId T::ID = std::hash<std::string>{}( \
    #T   \
    );

// #define INITIALISE_READERID _id = ID;

namespace TwinkleGraphics
{
class ResourceReader;
class ResourceManager;
typedef Singleton<ResourceManager> ResourceManagerInst;
typedef uint16_t ReaderId;
typedef uint64_t CacheId;

enum class CacheHint
{
    NONE_CACHE = 0,
    CACHE_SOURCE = 1,
    CACHE_OBJECT = 2
};

struct SourceHandle
{
    typedef std::shared_ptr<SourceHandle> Ptr;

    inline virtual CacheId GetCacheId() { return id; }

    CacheId id = 0;
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
    virtual ~ReaderOption()
    {}

    const ReaderOption& operator=(const ReaderOption& src)
    {
        _cacheHint = src._cacheHint;
        return *this;
    }

    inline void SetCacheHint(CacheHint hint) { _cacheHint = hint; }
    CacheHint GetCacheHint() { return _cacheHint; }

protected:
    CacheHint _cacheHint = CacheHint::CACHE_SOURCE;
};


class ResourceReader
{
public:
    typedef std::shared_ptr<ResourceReader> Ptr;

    ResourceReader()
        : _option(nullptr)
    {}
    ResourceReader(ReaderOption* option)
        : _option(option)
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

    // ReaderId GetReaderId() { return _id; };

protected:
    ReaderOption* _option = nullptr;
    // ReaderId _id = 0;;
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
        WAITLOAD,
        LOADING,
        SUCCESS,
        FAILED
    };

    ReadResult(Status status = Status::NONE)
        : _sharedObject(nullptr)
        , _status(status)
    {}
    ReadResult(TPtr obj, Status status = Status::NONE)
        : _sharedObject(obj)
        , _status(status)
    {}
    ReadResult(const ReadResult& src)
        : _sharedObject(src._sharedObject)
        , _status(src._status)
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

typedef void* VoidPtr;

class ResourceCache
{
public:
    typedef std::shared_ptr<ResourceCache> Ptr;

    ResourceCache(SourceHandle::Ptr source)
        : _sourceCache(source)
    {}
    ResourceCache(Object::Ptr obj)
        : _objectCache(obj)
    {}
    ~ResourceCache()
    {}

private:
    SourceHandle::Ptr _sourceCache;
    Object::Ptr _objectCache;

    friend class ResourceManager;
};


class ResourceManager
{
public:
    ResourceManager()
        : _threadPool(2)
    {}
    
    ~ResourceManager() 
    {
        _threadPool.Stop(true);
    }

    void Update()
    {
        ReadTask task;
        while (!_readTasks.EmptyNoLock())
        {
            _readTasks.Pop(task);
        }
    }

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
        // get GUID with filename, read from cache

        R* r = new R(std::forward<Args>(args)...);

        // http://klamp.works/2015/10/09/call-template-method-of-template-class-from-template-function.html
        return r->template Read<TPtr>(filename, option);
    }


    template<class R, class... Args>
    ReadResult<VoidPtr> ReadAsync(const char* filename, ReaderOption* option, Args&&...args)
    {
        // get GUID with filename, read from cache

        ReadTask task;
        task.filename = filename;
        task.option = option;
        ResourceReader::Ptr reader = GetIdleReader(R::ID);
        if(reader != nullptr)
        {
            reader = new (reader.get())R(std::forward<Args>(args)...);
        }
        else
        {
            R* r = new R(std::forward<Args>(args)...);
            reader = std::make_shared(r);
        }
        task.reader = reader;
        _readTasks.Push(task);

        return ReadResult<VoidPtr>(ReadResult<VoidPtr>::Status::WAITLOAD);
    }

    template <class R>
    void RecycleReader(typename R::Ptr reader)
    {
        if(reader != nullptr)
        {
            std::lock_guard<std::mutex> lock(_idleReaderMutex);
            {
                _idleReaders.insert(std::make_pair(R::ID, reader));
            }
        }
    }

private:
    ResourceReader::Ptr GetIdleReader(ReaderId id)
    {
        std::lock_guard<std::mutex> lock(_idleReaderMutex);
        {
            using MIterator = MultMapReaders::iterator;
            MIterator iter = _idleReaders.find(id);
            MIterator end = _idleReaders.end();
            if(iter != end)
            {
                ResourceReader::Ptr reader = iter->second;
                _idleReaders.erase(iter);

                return iter->second;
            }
        }

        return nullptr;
    }


private:
    typedef std::multimap<ReaderId, ResourceReader::Ptr> MultMapReaders;
    typedef std::unordered_map<CacheId, ResourceCache::Ptr> UnorderedCacheMap;
    typedef std::multimap<CacheId, ResourceCache::Ptr> MultCacheMap;

    struct ReadTask
    {
        std::string filename;
        ReaderOption *option = nullptr;
        ResourceReader::Ptr reader = nullptr;
    };

    TSQueue<ReadTask> _readTasks;

    MultCacheMap _objectCacheMap;
    UnorderedCacheMap _sourceCacheMap;

    // MultMapReaders _loadingReaders;
    MultMapReaders _idleReaders;

    // std::mutex _loadingReaderMutex;
    std::mutex _idleReaderMutex;

    ThreadPool _threadPool;
};


} // namespace TwinkleGraphics

#endif