#include "twResourceManager.h"
#include "twShader.h"

namespace TwinkleGraphics
{
    ResourceManager& ResourceMgrInstance() { return Singleton<ResourceManager>::Instance(); }

    // template <typename Ret, typename R>
    // void ResourceManager::PackedReadTask<Ret, R>::PushTask()
    // {
    //     // typedef Ret(R::*)(const char*, ReaderOption) Func;
    //     ResourceManager &resMgr = ResourceMgrInstance();
    //     auto future = resMgr.PushTask(&R::ReadAsync, _reader, _filename.c_str(), _option);
    // }
} // namespace TwinkleGraphics
