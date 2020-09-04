#ifndef TW_RESOURCEMANAGER_H
#define TW_RESOURCEMANAGER_H

#include <any>
#include <variant>

#include "twResource.h"

namespace TwinkleGraphics
{
    class __COMSINGLETONExport ResourceManager : public IUpdatable, public INonCopyable
    {
    public:
        virtual ~ResourceManager()
        {
            StopWorkers();
            _idleReaders.clear();
            _objectCacheMap.clear();
        }

        virtual void Update() override
        {

        }

        /**
     * @brief 
     * 
     * @tparam R 
     * @tparam TPtr 
     * @tparam Args 
     * @param filename 
     * @param option 
     * @param args 
     * @return ReadResult<TPtr> 
     */
        template <class R, class T, class... Args>
        ReadResult<T> Read(const char *filename, ReaderOption *option, Args &&... args)
        {
            // get GUID with filename, read from cache

            R *r = new R(std::forward<Args>(args)...);

            // http://klamp.works/2015/10/09/call-template-method-of-template-class-from-template-function.html
            return r->template Read<T>(filename, option);
        }

        template <class R, class T, class... Args>
        auto ReadAsync(const char *filename, ReaderOption *option, Args &&... args)
            -> std::future<ReadResult<T>>
        {
            // get GUID with filename, read from cache

            ResourceReader::Ptr reader = PopIdleReader(R::ID);
            R *r = nullptr;
            if (reader != nullptr)
            {
                r = new (reader.get()) R(std::forward<Args>(args)...);
                reader.reset(r);
            }
            else
            {
                r = new R(std::forward<Args>(args)...);
                reader.reset(r);
            }

            // typename PackedReadTask<ReadResult<T>>::Ptr packedReadTaskPtr = 
            //     std::make_shared<PackedReadTask<ReadResult<T>>>(ReadResult<T>());

            // std::shared_ptr<std::packaged_task<ReadResult<T>()>> task = packedReadTaskPtr->_task;
            // task = std::make_shared<std::packaged_task<ReadResult<T>()>>(
            //     std::bind(&R::ReadAsync, r, filename, option)
            // );

            // _taskQueue.Push(packedReadTaskPtr);

            // auto future = packedReadTaskPtr->_task->get_future();
            // return future;

            auto future = _threadPool.PushTask(&R::ReadAsync, r, filename, option);
            return future;
        }

        template <class R>
        void RecycleReader(typename R::Ptr reader)
        {
            if (reader != nullptr)
            {
                std::lock_guard<std::mutex> lock(_idleReaderMutex);
                {
                    _idleReaders.insert(std::make_pair(R::ID, reader));
                }
            }
        }

        void StopWorkers()
        {
            _threadPool.Stop(true);
        }

    private:
        explicit ResourceManager()
            : IUpdatable()
            , INonCopyable()
            , _threadPool(2)
        {
        }

        ResourceReader::Ptr PopIdleReader(ReaderId id)
        {
            std::lock_guard<std::mutex> lock(_idleReaderMutex);
            {
                using MIterator = MultMapReaders::iterator;
                MIterator iter = _idleReaders.find(id);
                MIterator end = _idleReaders.end();
                if (iter != end)
                {
                    ResourceReader::Ptr reader = iter->second;
                    _idleReaders.erase(iter);

                    return iter->second;
                }
            }

            return nullptr;
        }

    private:
        typedef std::multimap<ReaderId, ResourceReader::Ptr> MultMapReaders;
        typedef std::unordered_map<CacheId, ResourceCache::Ptr> UnorderedCacheMap;
        typedef std::multimap<CacheId, ResourceCache::Ptr> MultCacheMap;

        class IPackedReadTask
        {
        public:
            typedef std::shared_ptr<IPackedReadTask> Ptr;
            std::any _returnType;

            // template <typename ReturnType>
            // auto GetTask() ->
            //     std::shared_ptr<std::packaged_task<ReturnType()>>;
        };

        template <typename ReturnType>
        class PackedReadTask : public IPackedReadTask
        {
        public:
            typedef std::shared_ptr<PackedReadTask> Ptr;
            PackedReadTask(ReturnType ret)
                : IPackedReadTask()
            {
                _returnType = ret;
            }

            auto GetTask() -> 
                std::shared_ptr<std::packaged_task<ReturnType()>>
            {
                return _task;
            }

            std::shared_ptr<std::packaged_task<ReturnType()>> _task = nullptr;
        };

        TSQueue<IPackedReadTask::Ptr> _taskQueue;

        MultCacheMap _objectCacheMap;
        UnorderedCacheMap _sourceCacheMap;

        // MultMapReaders _loadingReaders;
        MultMapReaders _idleReaders;

        // std::mutex _loadingReaderMutex;
        std::mutex _idleReaderMutex;

        ThreadPool _threadPool;

        friend class Singleton<ResourceManager>;
    };


#ifdef __cplusplus
    extern "C"
    {
#endif
        __COMSINGLETONExport ResourceManager& ResourceMgrInstance();
#ifdef __cplusplus
    }
#endif

    typedef Singleton<ResourceManager> ResourceManagerInst;

} // namespace TwinkleGraphics

#endif
