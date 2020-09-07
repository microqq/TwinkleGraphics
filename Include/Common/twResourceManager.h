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
            ClearWorkerPool();
            _idleReaders.clear();
            _objectCacheMap.clear();
        }

        virtual void Update() override
        {
            IPackedReadTask::Ptr packedTask;
            while(_taskQueue.Pop(packedTask))
            {
                packedTask->PushTask();
            }
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

            ResourceReader::Ptr reader = PopIdleReader(R::ID);
            R *r = nullptr;
            if (reader != nullptr)
            {
                r = new (reader.get()) R(/*std::forward<Args>(args)...*/);
                reader.reset(r);
            }
            else
            {
                r = new R(/*std::forward<Args>(args)...*/);
                reader.reset(r);
            }

            // http://klamp.works/2015/10/09/call-template-method-of-template-class-from-template-function.html
            return r->template Read<T>(filename, option);
        }

        template <class R, class T, class... Args>
        void ReadAsync(const char *filename, ReaderOption *option, Args &&... args)
        // auto ReadAsync(const char *filename, ReaderOption *option, Args &&... args)
            //-> std::future<ReadResult<T>>
        {
            // get GUID with filename, read from cache

            ResourceReader::Ptr reader = PopIdleReader(R::ID);
            R *r = nullptr;
            if (reader != nullptr)
            {
                r = new (reader.get()) R(/*std::forward<Args>(args)...*/);
                reader.reset(r);
            }
            else
            {
                r = new R(/*std::forward<Args>(args)...*/);
                reader.reset(r);
            }

            typename PackedReadTask<ReadResult<T>, R>::Ptr packedReadTaskPtr = 
                std::make_shared<PackedReadTask<ReadResult<T>, R>>();

            packedReadTaskPtr->_filename = std::string(filename);
            packedReadTaskPtr->_reader = r;
            packedReadTaskPtr->_option = option;
            _taskQueue.Push(packedReadTaskPtr);

            // auto future = _workerPool.PushTask(&R::ReadAsync, r, filename, option);
            // return future;
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

        void ClearWorkerPool()
        {
            _workerPool.Stop(true);
        }

    private:
        explicit ResourceManager()
            : IUpdatable()
            , INonCopyable()
            , _workerPool(2)
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

        template <class Func, class... Args>
        auto PushTask(Func&& f, Args&&... args)
            -> std::future<typename std::result_of<Func(Args...)>::type>
        {
            auto future = _workerPool.PushTask(f, args...);
            return future;
        }

    private:
        typedef std::multimap<ReaderId, ResourceReader::Ptr> MultMapReaders;
        typedef std::unordered_map<CacheId, ResourceCache::Ptr> UnorderedCacheMap;
        typedef std::multimap<CacheId, ResourceCache::Ptr> MultCacheMap;
        typedef ThreadPool WorkerPool;

        class IPackedReadTask
        {
        public:
            typedef std::shared_ptr<IPackedReadTask> Ptr;

            virtual void PushTask() = 0;
        };

        template <typename Ret, typename R>
        class PackedReadTask : public IPackedReadTask
        {
        public:
            typedef std::shared_ptr<PackedReadTask> Ptr;
            PackedReadTask()
                : IPackedReadTask()
            {
            }
            virtual ~PackedReadTask()
            {
                _reader = nullptr;
                _option = nullptr;
            }

            virtual void PushTask() override;

            std::string _filename;
            R* _reader;
            ReaderOption* _option;
        };
        TSQueue<IPackedReadTask::Ptr> _taskQueue;

        MultCacheMap _objectCacheMap;
        UnorderedCacheMap _sourceCacheMap;

        // MultMapReaders _loadingReaders;
        MultMapReaders _idleReaders;

        // std::mutex _loadingReaderMutex;
        std::mutex _idleReaderMutex;

        WorkerPool _workerPool;

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
