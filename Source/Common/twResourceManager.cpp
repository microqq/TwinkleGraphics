#include "twResourceManager.h"
#include "twShader.h"

namespace TwinkleGraphics
{
    ResourceManager &ResourceMgrInstance() { return Singleton<ResourceManager>::Instance(); }

    ResourceManager::~ResourceManager()
    {
        Destroy();
    }

    void ResourceManager::Update(float deltaTime)
    {
        UpdateResourceCache(0.02f);

        int taskPerFrame = 4;
        IPackedReadTask::Ptr packedTask;
        while (_taskQueue.Pop(packedTask))
        {
            packedTask->PushTask();
            --taskPerFrame;

            if(taskPerFrame <= 0)
            {
                break;
            }
        }

        while(_cachedTaskQueue.Pop(packedTask))
        {
            packedTask->InvokeSuccess();
        }
    }

    void ResourceManager::Destroy()
    {
        ClearWorkerPool();

        IPackedReadTask::Ptr packedTask;
        while (_taskQueue.Pop(packedTask)) {}
        while (_cachedTaskQueue.Pop(packedTask)) {}

        std::lock_guard<std::mutex> lock(_readerMutex);
        {
            _idleReaders.clear();
            _loadingReaders.clear();
        }

        _sceneObjectsCacheMap.clear();
        _objectCacheMap.clear();
    }

    bool ResourceManager::AddResourceCache(CacheHint hint, ResourceCache::Ptr cache)
    {
        std::lock_guard<std::mutex> lock(_cacheMutex);

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

    void ResourceManager::UpdateResourceCache(float deltaTime)
    {
        std::lock_guard<std::mutex> lock(_cacheMutex);

        //remove expired cache
        for(auto it = _objectCacheMap.begin(); it != _objectCacheMap.end();)
        {
            if(it->second->Expired(deltaTime))
            {
                it = _objectCacheMap.erase(it);
                continue;
            }
            ++it;
        }

        for (auto it = _sceneObjectsCacheMap.begin(); it != _sceneObjectsCacheMap.end();)
        {
            if (it->second->Expired(deltaTime))
            {
                it = _sceneObjectsCacheMap.erase(it);
                continue;
            }
            ++it;
        }
    }

    ResourceCache::Ptr ResourceManager::GetResourceCache(CacheHint hint, CacheId id)
    {
        std::lock_guard<std::mutex> lock(_cacheMutex);

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

    void ResourceManager::OnReadTaskSuccess(ObjectPtr obj
        , ReadTaskId taskid
        , CacheHint cachehint
        , CacheStoreHint storeHint
        , CacheId cacheid
        , float storeTime
        , ReaderId readerid
        , ResourceReaderPtr reader
        )
    {
        ResourceCache::Ptr cache = std::make_shared<ResourceCache>(cacheid, obj, storeHint, storeTime);
        AddResourceCache(cachehint, cache);
        ReleaseTask(obj, taskid, true);
        RecycleReader(readerid, reader);
    }

    void ResourceManager::OnReadTaskFailed(ReadTaskId taskid, ReaderId readerid, ResourceReaderPtr reader)
    {
        ReleaseTask(nullptr, taskid, false);
        RecycleReader(readerid, reader);
    }

    void ResourceManager::RecycleReader(ReaderId id, ResourceReaderPtr reader)
    {
        std::lock_guard<std::mutex> lock(_readerMutex);

        auto range = _loadingReaders.equal_range(id);
        MultMapReaders::iterator iter = range.first;
        MultMapReaders::iterator second = range.second;

        while (iter != second)
        {
            if (iter->second == reader)
            {
                break;
            }
            ++iter;
        }

        if (iter != second)
        {
            reader->Reset();
            _loadingReaders.erase(iter);
            _idleReaders.insert(std::make_pair(id, reader));
        }
    }

    void ResourceManager::ReleaseTask(ObjectPtr obj, ReadTaskId taskid, bool success)
    {
        std::lock_guard<std::mutex> lock(_taskMutex);

        if (_loadingTasks.find(taskid) != _loadingTasks.end())
        {
            _loadingTasks.erase(taskid);
        }

        auto range = _waitToLoadTasks.equal_range(taskid);
        WaitToLoadTaskMap::iterator first = range.first;
        WaitToLoadTaskMap::iterator second = range.second;
        bool exist = first != second;

        IPackedReadTask::Ptr packedTask = nullptr;
        while (first != second)
        {
            packedTask = first->second;
            if (success)
            {
                packedTask->InvokeSuccess(obj);
            }
            else
            {
                packedTask->InvokeFailed();
            }

            ++first;
        }

        if (exist)
        {
            int eraseCount = _waitToLoadTasks.erase(taskid);
            Console::LogWithColor<Console::Color::GREEN>("Erased tasks (", taskid, ") that wait to load count ", eraseCount, ".\n");
        }
    }

} // namespace TwinkleGraphics
