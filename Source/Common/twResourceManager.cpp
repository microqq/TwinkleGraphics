#include "twResourceManager.h"
#include "twShader.h"

namespace TwinkleGraphics
{
    ResourceManager &ResourceMgrInstance() { return Singleton<ResourceManager>::Instance(); }

    ResourceManager::~ResourceManager()
    {
        ClearWorkerPool();
        _idleReaders.clear();
        _objectCacheMap.clear();
    }

    void ResourceManager::Update()
    {
        IPackedReadTask::Ptr packedTask;
        while (_taskQueue.Pop(packedTask))
        {
            packedTask->PushTask();
        }
    }

    void ResourceManager::ClearWorkerPool()
    {
        _workerPool.Stop(true);
    }

} // namespace TwinkleGraphics
