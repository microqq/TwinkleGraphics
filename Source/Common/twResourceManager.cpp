#include "twResourceManager.h"
#include "twShader.h"

namespace TwinkleGraphics
{
    ResourceManager &ResourceMgrInstance() { return Singleton<ResourceManager>::Instance(); }

    ResourceManager::~ResourceManager()
    {
        Destroy();
    }

    void ResourceManager::Update()
    {
        IPackedReadTask::Ptr packedTask;
        while (_taskQueue.Pop(packedTask))
        {
            packedTask->PushTask();
        }
    }

    void ResourceManager::Destroy()
    {
        ClearWorkerPool();
        IPackedReadTask::Ptr packedTask;
        while (_taskQueue.Pop(packedTask))
        {
        }
        _idleReaders.clear();
        _sceneObjectsCacheMap.clear();
    }

    bool ResourceManager::AddResourceCache(CacheHint hint, ResourceCache::Ptr cache)
    {
        if(CacheHint::CACHE_OBJECT == hint)
        {
            _objectCacheMap.insert(UnorderedCacheMap::value_type(cache->GetCacheId(), cache));
        }
        else if(CacheHint::CACHE_SCENEOBJECT == hint)
        {
            _sceneObjectsCacheMap.insert(MultCacheMap::value_type(cache->GetCacheId(), cache));
        } 

        return true;
    }

    bool ResourceManager::RemoveResourceCache(ResourceCache::Ptr cache)
    {
        return true;
    }

    ResourceCache::Ptr ResourceManager::GetResourceCache(CacheHint hint, CacheId id)
    {
        if(CacheHint::CACHE_OBJECT == hint)
        {
            UnorderedCacheMap::iterator iter = _objectCacheMap.find(id);
            if(iter != _objectCacheMap.end())
            {
                return iter->second;
            }
        }
        else if(CacheHint::CACHE_SCENEOBJECT == hint)
        {
            MultCacheMap::iterator iter = _sceneObjectsCacheMap.find(id);
            MultCacheMap::iterator end = _sceneObjectsCacheMap.end();

            if(iter != end)
            {
                return iter->second;
            }
        }

        return nullptr;
    }

    void ResourceManager::ClearWorkerPool()
    {
        _workerPool.Stop(true);
    }

} // namespace TwinkleGraphics
