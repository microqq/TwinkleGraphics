
#include "twResource.h"

namespace TwinkleGraphics
{
    ReaderOption::ReaderOption()
        : _cacheHint(CacheHint::CACHE_SOURCE)
        , _successFuncList()
        , _failedFuncList()
    {}

    ReaderOption::ReaderOption(const ReaderOption& src)
    {
        _cacheHint = src._cacheHint;
        _successFuncList = src._successFuncList;
        _failedFuncList = src._failedFuncList;
    }
    
    ReaderOption::~ReaderOption()
    {}

    void ReaderOption::SetCacheHint(CacheHint hint) { _cacheHint = hint; }
    CacheHint ReaderOption::GetCacheHint() { return _cacheHint; }

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










    ResourceReader::~ResourceReader() {}
    ResourceReader::ResourceReader() {}

    ReaderOption* ResourceReader::GetReaderOption() { return _option; }

} // namespace TwinkleGraphics
