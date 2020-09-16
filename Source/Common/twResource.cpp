
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
    {
        _successFuncList.clear();
        _failedFuncList.clear();
    }

    void ReaderOption::SetCacheHint(CacheHint hint) { _cacheHint = hint; }
    CacheHint ReaderOption::GetCacheHint() { return _cacheHint; }
    void ReaderOption::SetStoreHint(CacheStoreHint hint, float storeTime) 
    { 
        _storeHint = hint;
        _storeTime = storeTime; 
    }
    CacheStoreHint ReaderOption::GetStoreHint() { return _storeHint; }
    float ReaderOption::GetStoreTime() { return _storeTime; }
    
    void ReaderOption::AddSuccessFunc(int insertPos, ReadSuccessCallbackFuncPtr func)
    {
        if(insertPos == -1 || insertPos > _successFuncList.size() - 1)
        {
            _successFuncList.emplace_back(func);
        }
        else
        {
            using iterator = std::vector<ReadSuccessCallbackFuncPtr>::iterator;
            iterator it = _successFuncList.begin();

            it += insertPos;
            _successFuncList.insert(it, func);
        }
    }

    void ReaderOption::AddFailedFunc(int insertPos, ReadFailedCallbackFuncPtr func)
    {
        if(insertPos == -1 || insertPos > _successFuncList.size() - 1)
        {
            _failedFuncList.emplace_back(func);
        }
        else
        {
            using iterator = std::vector<ReadFailedCallbackFuncPtr>::iterator;
            iterator it = _failedFuncList.begin();

            it += insertPos;
            _failedFuncList.insert(it, func);
        }
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
        SAFE_DEL(_option);
    }
    ResourceReader::ResourceReader()
        : _option(nullptr)
    {}

    ReaderOption* ResourceReader::GetReaderOption() { return _option; }

    void ResourceReader::Reset()
    {
        SAFE_DEL(_option);
        _asynchronize = false;
    }

} // namespace TwinkleGraphics
