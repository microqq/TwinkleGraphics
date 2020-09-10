#ifndef TW_RESOURCEMANAGER_H
#define TW_RESOURCEMANAGER_H

#include <any>
#include <variant>
#include <set>

#include "twResource.h"

namespace TwinkleGraphics
{
    typedef uint64_t ReadTaskId;

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

            // typename PackedReadTask<ReadResult<T>, R>::Ptr packedReadTaskPtr =
            //     std::make_shared<PackedReadTask<ReadResult<T>, R>>(false);

            // packedReadTaskPtr->_filename = std::string(filename);
            // packedReadTaskPtr->_reader = r;
            // _taskQueue.Push(packedReadTaskPtr);

            CacheId id = CACHEID_FROMSTRING(filename);
            CacheHint hint = CacheHint::CACHE_NONE;
            if(option != nullptr)
            {
                hint = option->GetCacheHint();
            }
            ResourceCache::Ptr cache = GetResourceCache(hint, id);

            if(cache == nullptr)
            {
                typename R::Ptr reader = GetReader<R>(option);

                // http://klamp.works/2015/10/09/call-template-method-of-template-class-from-template-function.html
                ReadResult<T> result = reader->Read(filename);

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
        ReadResult<T> ReadAsync(const char *filename, ReaderOption *option, Args &&... args)
        // auto ReadAsync(const char *filename, ReaderOption *option, Args &&... args)
        //-> std::future<ReadResult<T>>
        {
            using Status = typename ReadResult<T>::Status;
            
            // Todo: get GUID with filename, read from cache

            CacheId id = CACHEID_FROMSTRING(filename);
            CacheHint hint = CacheHint::CACHE_NONE;
            if (option != nullptr)
            {
                hint = option->GetCacheHint();
            }
            ResourceCache::Ptr cache = GetResourceCache(hint, id);

            // get resource cache
            if (cache != nullptr)
            {
                typename T::Ptr obj = std::dynamic_pointer_cast<T>(cache->GetCachedObject());
                return ReadResult<T>(nullptr, obj, Status::SUCCESS);
            }

            // temporally use cacheid instead.
            ReadTaskId taskId = id;
            // packed read task
            typename R::Ptr reader = GetReader<R>(option);

            typename PackedReadTask<ReadResult<T>, R>::Ptr packedReadTaskPtr =
                std::make_shared<PackedReadTask<ReadResult<T>, R>>(taskId, true);
            packedReadTaskPtr->_filename = std::string(filename);
            packedReadTaskPtr->_reader = reader.get();
            
            {
                std::unique_lock<std::mutex> lock(_taskMutex, std::defer_lock);
                // task which has the same id is loading. 
                lock.lock();
                LoadingTaskSet::iterator iter = _loadingTasks.find(id);
                if(iter != _loadingTasks.end())
                {
                    _waitToLoadTasks.emplace(std::make_pair(taskId, packedReadTaskPtr));
                    lock.unlock();

                    Console::LogWarning("Readtask ", taskId, " ", filename, " wait to load.\n");

                    return ReadResult<T>(nullptr, nullptr, Status::WAITTOLOAD);
                }
                else
                {
                    ReaderOption* readerOption = reader->GetReaderOption();
                    if(readerOption != nullptr)
                    {
                        readerOption->AddSuccessFunc(this, &ResourceManager::OnReadTaskSuccess
                                        , taskId
                                        , readerOption->GetCacheHint()
                                        , id
                                        , R::ID
                                        , reader
                        );

                        readerOption->AddFailedFunc(this, &ResourceManager::OnReadTaskFailed
                                        , taskId
                                        , R::ID
                                        , reader
                        );
                    }

                    _loadingTasks.emplace(taskId);
                    lock.unlock();
 
                    _taskQueue.Push(packedReadTaskPtr);

                    return ReadResult<T>(nullptr, nullptr, Status::LOADING);
                }
            }
        }

    private:
        explicit ResourceManager()
            : IUpdatable()
            , INonCopyable()
            , _workerPool(2)
        {}

        template <typename R>
        typename R::Ptr GetReader(ReaderOption *option)
        {
            std::unique_lock<std::mutex> lock(_readerMutex, std::defer_lock);

            lock.lock();
            ResourceReader::Ptr reader = PopIdleReader(R::ID);
            lock.unlock();

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

            lock.lock();
            PushLoadingReader(R::ID, reader);
            lock.unlock();

            typename R::Ptr derivedReader = std::dynamic_pointer_cast<R>(reader);
            return derivedReader;
        }

        ResourceReader::Ptr PopIdleReader(ReaderId id)
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

            return nullptr;
        }

        void PushLoadingReader(ReaderId id, ResourceReader::Ptr reader)
        {
            _loadingReaders.insert(std::make_pair(id, reader));
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

        void OnReadTaskSuccess(Object::Ptr obj
            , ReadTaskId taskid
            , CacheHint cachehint
            , CacheId cacheid
            , ReaderId readerid
            , ResourceReader::Ptr reader
            )
        {
            ResourceCache::Ptr cache = std::make_shared<ResourceCache>(cacheid, obj);
            AddResourceCache(cachehint, cache);
            ReleaseTask(obj, taskid, true);
            RecycleReader(readerid, reader);
        }

        void OnReadTaskFailed(ReadTaskId taskid
            , ReaderId readerid
            , ResourceReader::Ptr reader
            )
        {
            ReleaseTask(nullptr, taskid, false);
            RecycleReader(readerid, reader);
        }

        void RecycleReader(ReaderId id, ResourceReader::Ptr reader)
        {
            std::lock_guard<std::mutex> lock(_readerMutex);

            auto range = _loadingReaders.equal_range(id);
            MultMapReaders::iterator iter = range.first;
            MultMapReaders::iterator second = range.second;
            if (iter != second)
            {
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
                    _loadingReaders.erase(iter);
                    _idleReaders.insert(std::make_pair(id, reader));
                }
            }
        }

        void ReleaseTask(Object::Ptr obj, ReadTaskId taskid, bool success)
        {
            std::lock_guard<std::mutex> lock(_taskMutex);

            if(_loadingTasks.find(taskid) != _loadingTasks.end())
            {
                _loadingTasks.erase(taskid);
            }

            auto range = _waitToLoadTasks.equal_range(taskid);
            WaitToLoadTaskMap::iterator first = range.first;
            WaitToLoadTaskMap::iterator second = range.second;
            bool exist = first != second;

            IPackedReadTask::Ptr packedTask = nullptr;
            while(first != second)
            {
                packedTask = first->second;
                if(success)
                {
                    packedTask->InvokeSuccess(obj);
                }
                else
                {
                    packedTask->InvokeFailed();
                }

                ++first;
            }

            if(exist)
            {
                int eraseCount = _waitToLoadTasks.erase(taskid);
                Console::LogWarning("Erased tasks which wait load count ", eraseCount, "\n");
            }
        }

    private:
        class IPackedReadTask
        {
        public:
            typedef std::shared_ptr<IPackedReadTask> Ptr;
            IPackedReadTask(ReadTaskId task, bool async = false)
                : _asyncRead(async)
                , _task(task)
            {}

            virtual void PushTask() = 0;

        protected:
            virtual void InvokeSuccess(Object::Ptr obj) = 0;
            virtual void InvokeFailed() = 0;

        protected:
            ReadTaskId _task = 0;;
            bool _asyncRead = false;

            friend class ResourceManager;
        };

        template <typename Ret, typename R>
        class __TWCOMExport PackedReadTask : public IPackedReadTask
        {
        public:
            typedef std::shared_ptr<PackedReadTask> Ptr;
            PackedReadTask(ReadTaskId task, bool async = false)
                : IPackedReadTask(task, async)
            {
            }
            virtual ~PackedReadTask()
            {
                _reader = nullptr;
            }

            virtual void PushTask() override;
        
        protected:
            virtual void InvokeSuccess(Object::Ptr obj) override
            {
                ReaderOption* option = _reader->GetReaderOption();
                if(option != nullptr)
                {
                    option->OnReadSuccess(obj);
                }
            }

            virtual void InvokeFailed() override
            {
                ReaderOption* option = _reader->GetReaderOption();
                if(option != nullptr)
                {
                    option->OnReadFailed();
                }                
            }

            std::string _filename;
            R *_reader;

            friend class ResourceManager;
        };

        typedef std::multimap<ReaderId, ResourceReader::Ptr> MultMapReaders;
#if defined _WIN32        
        typedef std::unordered_map<CacheId, ResourceCache::Ptr> UnorderedCacheMap;
#elif defined(__linux__) or defined(__APPLE__)
        typedef std::map<CacheId, ResourceCache::Ptr> UnorderedCacheMap;
#endif
        typedef std::multimap<CacheId, ResourceCache::Ptr> MultCacheMap;
        typedef ThreadPool WorkerPool;
        typedef std::set<ReadTaskId> LoadingTaskSet;
        typedef std::multimap<ReadTaskId, IPackedReadTask::Ptr> WaitToLoadTaskMap;

        TSQueue<IPackedReadTask::Ptr> _taskQueue;
        LoadingTaskSet _loadingTasks;
        WaitToLoadTaskMap _waitToLoadTasks;

        MultCacheMap _sceneObjectsCacheMap;
        UnorderedCacheMap _objectCacheMap;

        MultMapReaders _loadingReaders;
        MultMapReaders _idleReaders;

        std::mutex _taskMutex;
        std::mutex _readerMutex;

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
