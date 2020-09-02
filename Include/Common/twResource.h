

#ifndef TW_RESOURCE_H
#define TW_RESOURCE_H

#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <variant>
#include <vector>
#include <functional>

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
class ReaderOption;
class ResourceReader;
class ResourceManager;
typedef Singleton<ResourceManager> ResourceManagerInst;
typedef uint16_t ReaderId;
typedef uint64_t CacheId;

typedef std::function<void(Object::Ptr)> ReadSuccessFunc;
typedef std::function<void()> ReadFailedFunc;
typedef std::shared_ptr<ReadSuccessFunc> ReadSuccessFuncPtr;
typedef std::shared_ptr<ReadFailedFunc> ReadFailedFuncPtr;

enum class CacheHint
{
    NONE_CACHE = 0,
    CACHE_SOURCE = 1,
    CACHE_OBJECT = 2
};

class SourceHandle : public Object
{
public:
    typedef std::shared_ptr<SourceHandle> Ptr;

    inline virtual CacheId GetCacheId() { return _id; }

private:
    CacheId _id = 0;
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


/**
 * @brief 
 * 
 * @tparam TPtr 
 */
template<class T>
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
    ReadResult(typename T::Ptr obj, Status status = Status::NONE)
        : _sharedObject(obj)
        , _status(status)
    {}
    ReadResult(const ReadResult& src)
        : _sharedObject(src._sharedObject)
        , _status(src._status)
    {
        _successFuncList = src._successFuncList;
        _failedFuncList = src._failedFuncList;
    }
    ~ReadResult()
    {}

    inline typename T::Ptr GetSharedObject() const { return _sharedObject; }
    inline Status GetStatus() const { return _status; }
    inline ReadResult& operator=(const ReadResult& result)
    {
        _sharedObject = result._sharedObject;
        _status = result._status;

        return *this;
    }

    inline bool operator==(std::nullptr_t nullp) const
    {
        return _sharedObject == nullp;
    }

    void AddSuccessFunc(ReadSuccessFuncPtr func)
    {
        _successFuncList.emplace_back(func);
    }

    void AddFailedFunc(ReadFailedFuncPtr func)
    {
        _failedFuncList.emplace_back(func);
    }

    void OnReadSuccess() const
    {
        if(Status::SUCCESS == _status)
        {
            for(auto& func : _successFuncList)
            {
                (*func)(_sharedObject);
            }
        }
    }

    void OnReadFailed() const
    {
        if(Status::FAILED == _status)
        {
            for (auto &func : _failedFuncList)
            {
                (*func)();
            }
        }
    }

private:
    std::vector<ReadSuccessFuncPtr> _successFuncList;
    std::vector<ReadFailedFuncPtr> _failedFuncList;
    typename T::Ptr _sharedObject;
    Status _status;
};


template <typename T>
bool ReadFinished(std::future<ReadResult<T>> const &f)
{
    using namespace std::chrono_literals;
    return f.wait_for(std::chrono::microseconds(0ms)) == std::future_status::ready;
}

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
    ResourceReader(const ResourceReader& src)
        : _option(src._option)
        , _asynchronize(src._asynchronize)
    {

    }
    ResourceReader(ResourceReader&& src)
        : _option(std::move(src._option))
        , _asynchronize(src._asynchronize)
    {

    }

    virtual ~ResourceReader() 
    {
        SAFE_DEL(_option);
    }

    template <typename T>
    void SetReaderOption(T* option) 
    {
        if(option == nullptr)
        {
            return;
        }

        _option = new T(*option);
    }
    const ReaderOption* const GetReaderOption() {  return this->_option; }

protected:
    ReaderOption* _option = nullptr;
    bool _asynchronize = false;

    friend class ResourceManager;
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

class ResourceManager : public IUpdatable
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
    template<class R, class T, class... Args>
    ReadResult<T> Read(const char* filename, ReaderOption* option, Args&&...args)
    {
        // get GUID with filename, read from cache

        R* r = new R(std::forward<Args>(args)...);

        // http://klamp.works/2015/10/09/call-template-method-of-template-class-from-template-function.html
        return r->template Read<T>(filename, option);
    }


    template<class R, class T, class... Args>
    auto ReadAsync(const char* filename, ReaderOption* option, Args&&...args)
        -> std::future<ReadResult<T>>
    {
        // get GUID with filename, read from cache

        ResourceReader::Ptr reader = PopIdleReader(R::ID);
        R* r = nullptr;
        if(reader != nullptr)
        {
            r = new (reader.get())R(std::forward<Args>(args)...);
            reader.reset(r);
        }
        else
        {
            r = new R(std::forward<Args>(args)...);
            reader.reset(r);
        }
        
        auto future = _threadPool.PushTask(&R::ReadAsync, r, filename, option);
        return future;
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
    ResourceReader::Ptr PopIdleReader(ReaderId id)
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