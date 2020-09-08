
#include "twResource.h"

namespace TwinkleGraphics
{
    ReaderOption::ReaderOption()
        : _cacheHint(CacheHint::CACHE_SOURCE)
    {}

    ReaderOption::ReaderOption(const ReaderOption& src)
    {
        _cacheHint = src._cacheHint;
    }
    
    ReaderOption::~ReaderOption()
    {}

    void ReaderOption::SetCacheHint(CacheHint hint) { _cacheHint = hint; }
    CacheHint ReaderOption::GetCacheHint() { return _cacheHint; }

    ResourceReader::~ResourceReader() {}
    ResourceReader::ResourceReader() {}

} // namespace TwinkleGraphics
