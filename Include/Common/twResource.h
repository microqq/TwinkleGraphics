

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
typedef uint16_t ReaderId;
typedef uint64_t CacheId;

typedef std::function<void()> ReadSuccessCallbackFunc;
typedef ReadSuccessCallbackFunc ReadFailedCallbackFunc;
typedef std::shared_ptr<ReadSuccessCallbackFunc> ReadSuccessCallbackFuncPtr;
typedef std::shared_ptr<ReadFailedCallbackFunc> ReadFailedCallbackFuncPtr;

enum class CacheHint
{
    NONE_CACHE = 0,
    CACHE_SOURCE = 1,
    CACHE_OBJECT = 2
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

protected:
    CacheHint _cacheHint = CacheHint::CACHE_SOURCE;
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
    // ReadResult(ReadResult&& src)
    // {
    // }
    ~ReadResult()
    {}

    inline ReadResult& operator=(const ReadResult& result)
    {
        _sharedObject = result._sharedObject;
        _status = result._status;

        return *this;
    }
    // inline ReadResult& operator=(ReadResult&& result)
    // {
    //     return *this;
    // }

    inline typename T::Ptr GetSharedObject() const { return _sharedObject; }
    inline Status GetStatus() const { return _status; }

    // inline bool operator==(std::nullptr_t nullp) const
    // {
    //     return _sharedObject == nullp;
    // }

    template <typename Func, typename... Args>
    void AddSuccessFunc(Func&& func, Args&&...args)
    {
        auto concreteCallback = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
        ReadSuccessCallbackFuncPtr callback = std::make_shared<ReadSuccessCallbackFunc>(
            [concreteCallback]()
            {
                concreteCallback();
            }
        );

        AddSuccessFunc(callback);
    }

    template <typename Func, typename... Args>
    void AddFailedFunc(Func&& func, Args&&...args)
    {
        auto concreteCallback = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
        ReadFailedCallbackFuncPtr callback = std::make_shared<ReadFailedCallbackFunc>(
            [concreteCallback]()
            {
                concreteCallback();
            }
        );

        AddFailedFunc(callback);
    }

    void AddSuccessFunc(ReadSuccessCallbackFuncPtr func)
    {
        _successFuncList.emplace_back(func);
    }

    void AddFailedFunc(ReadFailedCallbackFuncPtr func)
    {
        _failedFuncList.emplace_back(func);
    }

    void OnReadSuccess() const
    {
        if(Status::SUCCESS == _status)
        {
            for(auto& func : _successFuncList)
            {
                (*func)();
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
    std::vector<ReadSuccessCallbackFuncPtr> _successFuncList;
    std::vector<ReadFailedCallbackFuncPtr> _failedFuncList;
    typename T::Ptr _sharedObject;
    Status _status;
};

class ResourceReader : public INonCopyable
{
public:
    typedef std::shared_ptr<ResourceReader> Ptr;

    virtual ~ResourceReader();

protected:
    ResourceReader();

protected:
    bool _asynchronize = false;
};

class ResourceCache
{
public:
    typedef std::shared_ptr<ResourceCache> Ptr;

    ResourceCache(Object::Ptr obj)
        : _objectCache(obj)
    {}
    ~ResourceCache()
    {}

private:
    Object::Ptr _objectCache;
};


} // namespace TwinkleGraphics

#endif