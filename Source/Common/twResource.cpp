
#include "twResource.h"

namespace TwinkleGraphics
{
    ReaderOption::ReaderOption()
        : _cacheHint(CacheHint::CACHE_OBJECT)
        , _successFuncList()
        , _failedFuncList()
        , _storeHint(CacheStoreHint::TIMELIMITED)
        , _storeTime(100.0f)
    {}

    ReaderOption::ReaderOption(const ReaderOption& src)
    {
        _cacheHint = src._cacheHint;
        _successFuncList = src._successFuncList;
        _failedFuncList = src._failedFuncList;
        _storeHint = src._storeHint;
        _storeTime = src._storeTime;
    }
    
    ReaderOption::~ReaderOption()
    {}

    void ReaderOption::SetCacheHint(CacheHint hint) { _cacheHint = hint; }
    CacheHint ReaderOption::GetCacheHint() { return _cacheHint; }
    void ReaderOption::SetStoreHint(CacheStoreHint hint, float storeTime) 
    { 
        _storeHint = hint;
        _storeTime = storeTime; 
    }
    CacheStoreHint ReaderOption::GetStoreHint() { return _storeHint; }
    float ReaderOption::GetStoreTime() { return _storeTime; }
    
    void ReaderOption::AddSuccessFunc(ReadSuccessCallbackFuncPtr func)
    {
        _successFuncList.emplace_back(func);
    }

    void ReaderOption::AddFailedFunc(ReadFailedCallbackFuncPtr func)
    {
        _failedFuncList.emplace_back(func);
    }

    void ReaderOption::OnReadSuccess(Object::Ptr obj) const
    {
        for (auto &func : _successFuncList)
        {
            (*func)(obj);
        }
    }

    void ReaderOption::OnReadFailed() const
    {
        for (auto &func : _failedFuncList)
        {
            (*func)();
        }
    }










    ResourceReader::~ResourceReader() 
    {
        _option = nullptr;
    }
    ResourceReader::ResourceReader() {}

    ReaderOption* ResourceReader::GetReaderOption() { return _option; }

} // namespace TwinkleGraphics
