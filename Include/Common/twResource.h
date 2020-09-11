

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
#define CACHEID_FROMSTRING(STR) std::hash<std::string>{}(STR);

// #define INITIALISE_READERID _id = ID;

namespace TwinkleGraphics
{
class ReaderOption;
class ResourceReader;
typedef uint16_t ReaderId;
typedef uint64_t CacheId;

typedef std::function<void(Object::Ptr)> ReadSuccessCallbackFunc;
typedef std::function<void()> ReadFailedCallbackFunc;
typedef std::shared_ptr<ReadSuccessCallbackFunc> ReadSuccessCallbackFuncPtr;
typedef std::shared_ptr<ReadFailedCallbackFunc> ReadFailedCallbackFuncPtr;

enum class CacheHint
{
    CACHE_NONE = 0,
    CACHE_OBJECT = 1,
    CACHE_SCENEOBJECT = 2
};

enum class CacheStoreHint
{
    TEMPORARY = 1,
    TIMELIMITED = 2,
    PERMERNANTLY = 3
};

class __TWCOMExport ReaderOption
{
public:
    ReaderOption();
    ReaderOption(const ReaderOption& src);
    const ReaderOption& operator=(const ReaderOption& src) = delete;
    virtual ~ReaderOption();

    void SetCacheHint(CacheHint hint);
    CacheHint GetCacheHint();

    void SetStoreHint(CacheStoreHint hint, float storeTime = 100.0f);
    CacheStoreHint GetStoreHint();
    float GetStoreTime();

    template <typename Caller, typename Func, typename... Args>
    void AddSuccessFunc(Caller&& caller, Func&& func, Args&&...args)
    {
        auto concreteCallback = std::bind(
            std::forward<Func>(func)
            , std::forward<Caller>(caller)
            , std::placeholders::_1
            , std::forward<Args>(args)...
        );
        ReadSuccessCallbackFuncPtr callback = std::make_shared<ReadSuccessCallbackFunc>(
            [concreteCallback](Object::Ptr obj)
            {
                concreteCallback(obj);
            }
        );

        AddSuccessFunc(callback);
    }

    template <typename Caller, typename Func, typename... Args>
    void AddFailedFunc(Caller&& caller, Func&& func, Args&&...args)
    {
        auto concreteCallback = std::bind(
            std::forward<Func>(func)
            , std::forward<Caller>(caller)
            , std::forward<Args>(args)...
        );
        ReadFailedCallbackFuncPtr callback = std::make_shared<ReadFailedCallbackFunc>(
            [concreteCallback]()
            {
                concreteCallback();
            }
        );

        AddFailedFunc(callback);
    }

    void AddSuccessFunc(ReadSuccessCallbackFuncPtr func);
    void AddFailedFunc(ReadFailedCallbackFuncPtr func);
    void OnReadSuccess(Object::Ptr obj) const;
    void OnReadFailed() const;

protected:
    std::vector<ReadSuccessCallbackFuncPtr> _successFuncList;
    std::vector<ReadFailedCallbackFuncPtr> _failedFuncList;
    CacheHint _cacheHint = CacheHint::CACHE_OBJECT;
    CacheStoreHint _storeHint = CacheStoreHint::TIMELIMITED;
    float _storeTime;
};

class ResourceReader
{
public:
    typedef std::shared_ptr<ResourceReader> Ptr;

    virtual ~ResourceReader();
    ReaderOption* GetReaderOption();

protected:
    ResourceReader();

protected:
    ReaderOption* _option = nullptr;
    bool _asynchronize = false;
};

/**
 * @brief 
 * 
 * @tparam TPtr 
 */
template<class T>
class __TWCOMExport ReadResult
{
public:
    enum class Status
    {
        NONE,
        WAITTOLOAD,
        LOADING,
        SUCCESS,
        FAILED
    };

    ReadResult(Status status = Status::NONE)
        : _sharedObject(nullptr)
        , _reader(nullptr)
        , _status(status)
    {}
    ReadResult(ResourceReader::Ptr reader, typename T::Ptr obj, Status status = Status::NONE)
        : _sharedObject(obj)
        , _reader(reader)
        , _status(status)
    {}
    ReadResult(const ReadResult& src)
        : _sharedObject(src._sharedObject)
        , _reader(src._reader)
        , _status(src._status)
    {
    }
    ~ReadResult()
    {}

    inline ReadResult& operator=(const ReadResult& result)
    {
        _sharedObject = result._sharedObject;
        _status = result._status;

        return *this;
    }

    inline typename T::Ptr GetSharedObject() const { return _sharedObject; }
    inline Status GetStatus() const { return _status; }
    inline ResourceReader::Ptr GetReader() { return _reader; }

private:
    typename T::Ptr _sharedObject;
    ResourceReader::Ptr _reader;
    Status _status;
};

class ResourceCache : public INonCopyable
{
public:
    typedef std::shared_ptr<ResourceCache> Ptr;

    ResourceCache(CacheId id
            , Object::Ptr obj
            , CacheStoreHint hint = CacheStoreHint::TIMELIMITED
            , float limit = 100.0f)
        : _cachedObject(obj)
        , _cacheId(id)
        , _storeHint(hint)
        , _timeLimit(limit)
    {}

    ~ResourceCache()
    {}

    Object::Ptr GetCachedObject()
    {
        return _cachedObject;
    }

    CacheId GetCacheId() 
    {
        return _cacheId;
    }

    bool Expired(float deltaTime)
    {
        if(CacheStoreHint::PERMERNANTLY == _storeHint)
        {
            return true;
        }
        else if(CacheStoreHint::TIMELIMITED == _storeHint)
        {
            _timeLimit -= deltaTime;
            return _timeLimit <= 0.0f;
        }

        return true;
    }

private:
    Object::Ptr _cachedObject = nullptr;

    // cache life time(seconds)
    float _timeLimit;
    CacheStoreHint _storeHint = CacheStoreHint::TIMELIMITED;
 
    CacheId _cacheId = 0;
};


} // namespace TwinkleGraphics

#endif