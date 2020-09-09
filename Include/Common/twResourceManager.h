#ifndef TW_RESOURCEMANAGER_H
#define TW_RESOURCEMANAGER_H

#include <any>
#include <variant>

#include "twResource.h"

namespace TwinkleGraphics
{
    class __TWCOMExport ResourceManager : public IUpdatable
        , public INonCopyable
        , public IDestroyable
    {
    public:
        virtual ~ResourceManager();
        virtual void Update() override;
        virtual void Destroy() override;

        bool AddResourceCache(CacheHint hint, ResourceCache::Ptr cache);
        bool RemoveResourceCache(ResourceCache::Ptr cache);
        ResourceCache::Ptr GetResourceCache(CacheHint hint, CacheId id);

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

            R* r = GetReader<R>(option);

            // typename PackedReadTask<ReadResult<T>, R>::Ptr packedReadTaskPtr =
            //     std::make_shared<PackedReadTask<ReadResult<T>, R>>(false);

            // packedReadTaskPtr->_filename = std::string(filename);
            // packedReadTaskPtr->_reader = r;
            // _taskQueue.Push(packedReadTaskPtr);

            CacheId id = CACHEID_FROMSTRING(filename);
            CacheHint hint = CacheHint::CACHE_OBJECT;
            if(option != nullptr)
            {
                hint = option->GetCacheHint();
            }
            ResourceCache::Ptr cache = GetResourceCache(hint, id);
            if(cache == nullptr)
            {
                // http://klamp.works/2015/10/09/call-template-method-of-template-class-from-template-function.html
                ReadResult<T> result = r->template Read(filename);

                // add resource cache
                typename T::Ptr obj = result.GetSharedObject();
                ResourceCache::Ptr cache = std::make_shared<ResourceCache>(id, obj);
                AddResourceCache(hint, cache);

                return result;
            }
            else
            {
                using Status = typename ReadResult<T>::Status;
                typename T::Ptr obj = std::dynamic_pointer_cast<T>(cache->GetCachedObject());
                return ReadResult<T>(nullptr, obj, Status::SUCCESS);
            }
        }

        template <class R, class T, class... Args>
        void ReadAsync(const char *filename, ReaderOption *option, Args &&... args)
        // auto ReadAsync(const char *filename, ReaderOption *option, Args &&... args)
        //-> std::future<ReadResult<T>>
        {
            // get GUID with filename, read from cache

            R* r = GetReader<R>(option);

            typename PackedReadTask<ReadResult<T>, R>::Ptr packedReadTaskPtr =
                std::make_shared<PackedReadTask<ReadResult<T>, R>>(true);

            packedReadTaskPtr->_filename = std::string(filename);
            packedReadTaskPtr->_reader = r;
            _taskQueue.Push(packedReadTaskPtr);
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

    private:
        explicit ResourceManager()
            : IUpdatable(), INonCopyable(), _workerPool(2)
        {
        }

        template <typename R>
        R* GetReader(ReaderOption *option)
        {
            ResourceReader::Ptr reader = PopIdleReader(R::ID);
            R *r = nullptr;
            if (reader != nullptr)
            {
                r = new (reader.get()) R(option);
                reader.reset(r);
            }
            else
            {
                r = new R(option);
                reader.reset(r);
            }
            PushLoadingReader(R::ID, reader);

            return r;
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

        void PushLoadingReader(ReaderId id, ResourceReader::Ptr reader)
        {
            std::lock_guard<std::mutex> lock(_loadingReaderMutex);
            {
                _loadingReaders.insert(std::make_pair(id, reader));
            }
        }

        template <class Func, class... Args>
        void PushTask(Func &&f, Args &&... args)
        {
            auto read = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);
            read();
        }

        template <class Func, class... Args>
        auto PushAsyncTask(Func &&f, Args &&... args)
            -> std::future<typename std::result_of<Func(Args...)>::type>
        {
            auto future = _workerPool.PushTask(std::forward<Func>(f), std::forward<Args>(args)...);
            return future;
        }

        void ClearWorkerPool();

    private:
        typedef std::multimap<ReaderId, ResourceReader::Ptr> MultMapReaders;
        typedef std::unordered_map<CacheId, ResourceCache::Ptr> UnorderedCacheMap;
        typedef std::multimap<CacheId, ResourceCache::Ptr> MultCacheMap;
        typedef ThreadPool WorkerPool;

        class IPackedReadTask
        {
        public:
            typedef std::shared_ptr<IPackedReadTask> Ptr;
            IPackedReadTask(bool async = false)
                : _asyncRead(async)
            {}

            virtual void PushTask() = 0;

        protected:
            bool _asyncRead = false;
        };

        template <typename Ret, typename R>
        class __TWCOMExport PackedReadTask : public IPackedReadTask
        {
        public:
            typedef std::shared_ptr<PackedReadTask> Ptr;
            PackedReadTask(bool async = false)
                : IPackedReadTask(async)
            {
            }
            virtual ~PackedReadTask()
            {
                _reader = nullptr;
            }

            virtual void PushTask() override;

            std::string _filename;
            R *_reader;
        };
        TSQueue<IPackedReadTask::Ptr> _taskQueue;

        MultCacheMap _sceneObjectsCacheMap;
        UnorderedCacheMap _objectCacheMap;

        MultMapReaders _loadingReaders;
        MultMapReaders _idleReaders;

        std::mutex _loadingReaderMutex;
        std::mutex _idleReaderMutex;

        WorkerPool _workerPool;

        friend class Singleton<ResourceManager>;
    };

#ifdef __cplusplus
    extern "C"
    {
#endif
        __TWCOMExport ResourceManager &ResourceMgrInstance();
#ifdef __cplusplus
    }
#endif

    typedef Singleton<ResourceManager> ResourceManagerInst;

} // namespace TwinkleGraphics

#endif
