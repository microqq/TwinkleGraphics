#ifndef TW_RESOURCEMANAGER_H
#define TW_RESOURCEMANAGER_H

#include <set>

#include "twResource.h"

namespace TwinkleGraphics {
class ResourceManager;
__TWAPI ResourceManager &ResourceMgrInstance();

using ReadTaskId = uint64_t;

class __TWAPI ResourceManager : public IUpdatable,
                                      public INonCopyable,
                                      public IDestroyable {
public:
  virtual ~ResourceManager();

  /**
   * @brief
   * Update() must execute in main thread
   */
  virtual void Update(float deltaTime = 0.0f) override;
  virtual void Destroy() override;

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
  template <class R, class T, class OP, class... Args>
  ReadResult<T> Read(const char *filename, OP *option, Args &&...args) {
    // get GUID with filename, read from cache

    // typename PackedReadTask<ReadResult<T>, R>::Ptr packedReadTaskPtr =
    //     std::make_shared<PackedReadTask<ReadResult<T>, R>>(false);

    // packedReadTaskPtr->_filename = std::string(filename);
    // packedReadTaskPtr->_reader = r;
    // _taskQueue.Push(packedReadTaskPtr);

    CacheId id = CACHEID_FROMSTRING(filename);
    CacheHint hint = CacheHint::CACHE_NONE;
    if (option != nullptr) {
      hint = option->GetCacheHint();
    }
    ResourceCache::Ptr cache = GetResourceCache(hint, id);

    if (cache == nullptr) {
      typename R::Ptr reader = GetReader<R>(option);

      // http://klamp.works/2015/10/09/call-template-method-of-template-class-from-template-function.html
      ReadResult<T> result = reader->Read(filename);

      // add resource cache
      typename T::Ptr obj = result.GetSharedObject();
      ResourceCache::Ptr cache = std::make_shared<ResourceCache>(
          id, obj,
          option != nullptr ? option->GetStoreHint()
                            : CacheStoreHint::TIMELIMITED,
          option != nullptr ? option->GetStoreTime() : 100.0f);
      AddResourceCache(hint, cache);

      RecycleReader(R::ID, reader);

      return result;
    } else {
      using Status = typename ReadResult<T>::Status;
      typename T::Ptr obj =
          std::dynamic_pointer_cast<T>(cache->GetCachedObject());
      return ReadResult<T>(nullptr, obj, Status::SUCCESS);
    }
  }

  template <class R, class T, class OP, class... Args>
  ReadResult<T> ReadAsync(const char *filename, OP *option, Args &&...args)
  // auto ReadAsync(const char *filename, ReaderOption *option, Args &&... args)
  //-> std::future<ReadResult<T>>
  {
    using Status = typename ReadResult<T>::Status;

    // Todo: get GUID with filename, read from cache
    CacheId id = CACHEID_FROMSTRING(filename);

    // temporally use cacheid instead.
    // packed read task
    ReadTaskId taskId = id;
    typename R::Ptr reader = GetReader<R>(option);
    typename PackedReadTask<ReadResult<T>, R>::Ptr packedReadTaskPtr =
        std::make_shared<PackedReadTask<ReadResult<T>, R>>(taskId, true);
    packedReadTaskPtr->_filename = std::string(filename);
    packedReadTaskPtr->_reader = reader.get();

    CacheHint hint = CacheHint::CACHE_NONE;
    if (option != nullptr) {
      hint = option->GetCacheHint();
    }
    // get resource cache
    ResourceCache::Ptr cache = GetResourceCache(hint, id);
    if (cache != nullptr) {
      ReaderOption *readerOption = reader->GetReaderOption();
      readerOption->AddSuccessFunc(-1, this, &ResourceManager::OnRecycleReader,
                                   R::ID, reader);

      _cachedTaskQueue.Push(packedReadTaskPtr);
      typename T::Ptr obj =
          std::dynamic_pointer_cast<T>(cache->GetCachedObject());
      return ReadResult<T>(nullptr, obj, Status::SUCCESS);
    }

    {
      std::unique_lock<std::mutex> lock(_taskMutex, std::defer_lock);
      // task which has the same id is loading.
      lock.lock();
      LoadingTaskSet::iterator iter = _loadingTasks.find(id);
      if (iter != _loadingTasks.end()) {
        ReaderOption *readerOption = reader->GetReaderOption();
        readerOption->AddSuccessFunc(
            -1, this, &ResourceManager::OnRecycleReader, R::ID, reader);
        _waitToLoadTasks.emplace(std::make_pair(taskId, packedReadTaskPtr));
        lock.unlock();

        Console::LogWithColor<Console::Color::GREEN>(
            "Readtask ", taskId, " ", filename, " wait to load.\n");

        return ReadResult<T>(nullptr, nullptr, Status::WAITTOLOAD);
      } else {
        ReaderOption *readerOption = reader->GetReaderOption();
        if (readerOption != nullptr) {
          readerOption->AddSuccessFunc(
              -1, this, &ResourceManager::OnReadTaskSuccess, taskId,
              readerOption->GetCacheHint(), readerOption->GetStoreHint(), id,
              readerOption->GetStoreTime(), R::ID, reader);

          readerOption->AddFailedFunc(-1, this,
                                      &ResourceManager::OnReadTaskFailed,
                                      taskId, R::ID, reader);
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
      : IUpdatable(), INonCopyable(), IDestroyable(), _workerPool(4) {}

  template <typename R, typename OP> typename R::Ptr GetReader(OP *option) {
    std::unique_lock<std::mutex> lock(_readerMutex, std::defer_lock);

    lock.lock();
    ResourceReaderPtr reader = PopIdleReader(R::ID);
    lock.unlock();

    if (reader != nullptr) {
      typename R::Ptr rPtr = std::dynamic_pointer_cast<R>(reader);
      rPtr->SetOption(option);

      // r = new (reader.get()) R(option);
      // reader.reset(r);
    } else {
      reader = std::make_shared<R>(option);
    }

    lock.lock();
    PushLoadingReader(R::ID, reader);
    lock.unlock();

    typename R::Ptr derivedReader = std::dynamic_pointer_cast<R>(reader);
    return derivedReader;
  }

  ResourceReaderPtr PopIdleReader(ReaderId id) {
    using MIterator = MultMapReaders::iterator;
    MIterator iter = _idleReaders.find(id);
    MIterator end = _idleReaders.end();
    if (iter != end) {
      ResourceReaderPtr reader = iter->second;
      _idleReaders.erase(iter);

      return reader;
    }

    return nullptr;
  }

  void PushLoadingReader(ReaderId id, ResourceReaderPtr reader) {
    _loadingReaders.insert(std::make_pair(id, reader));
  }

  template <class Func, class... Args> void PushTask(Func &&f, Args &&...args) {
    auto read = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);
    read();
  }

  template <class Func, class... Args>
  auto PushAsyncTask(Func &&f, Args &&...args)
      -> std::future<typename std::result_of<Func(Args...)>::type> {
    auto future = _workerPool.PushTask(std::forward<Func>(f),
                                       std::forward<Args>(args)...);
    return future;
  }

  void ClearWorkerPool();

  void OnReadTaskSuccess(ObjectPtr obj, ReadTaskId taskid, CacheHint cachehint,
                         CacheStoreHint storeHint, CacheId cacheid,
                         float storeTime, ReaderId readerid,
                         ResourceReaderPtr reader);
  void OnReadTaskFailed(ReadTaskId taskid, ReaderId readerid,
                        ResourceReaderPtr reader);

  void OnRecycleReader(ObjectPtr obj, ReaderId id, ResourceReaderPtr reader) {
    RecycleReader(id, reader);
  }

  void RecycleReader(ReaderId id, ResourceReaderPtr reader);
  void ReleaseTask(ObjectPtr obj, ReadTaskId taskid, bool success);

  bool AddResourceCache(CacheHint hint, ResourceCache::Ptr cache);
  ResourceCache::Ptr GetResourceCache(CacheHint hint, CacheId id);
  void UpdateResourceCache(float deltaTime = 0.0f);

private:
  class IPackedReadTask {
  public:
    using Ptr = std::shared_ptr<IPackedReadTask>;
    IPackedReadTask(ReadTaskId task, bool async = false)
        : _task(task), _asyncRead(async) {}

    virtual void PushTask() = 0;

  protected:
    virtual void InvokeSuccess() = 0;
    virtual void InvokeSuccess(ObjectPtr obj) = 0;
    virtual void InvokeFailed() = 0;

  protected:
    std::string _filename;
    ReadTaskId _task = 0;
    ;
    bool _asyncRead = false;

    friend class ResourceManager;
  };

  template <typename Ret, typename R>
  class __TWAPI PackedReadTask : public IPackedReadTask {
  public:
    using Ptr = std::shared_ptr<PackedReadTask>;
    PackedReadTask(ReadTaskId task, bool async = false)
        : IPackedReadTask(task, async) {}
    virtual ~PackedReadTask() { _reader = nullptr; }

    virtual void PushTask() override;

  protected:
    virtual void InvokeSuccess() override {
      ResourceManager &resMgr = ResourceMgrInstance();
      CacheId cacheid = CACHEID_FROMSTRING(_filename);
      ReaderOption *option = _reader->GetReaderOption();
      if (option != nullptr) {
        ResourceCache::Ptr cache =
            resMgr.GetResourceCache(option->GetCacheHint(), cacheid);
        option->OnReadSuccess(cache == nullptr ? nullptr
                                               : cache->GetCachedObject());
      }
    }

    virtual void InvokeSuccess(ObjectPtr obj) override {
      ReaderOption *option = _reader->GetReaderOption();
      if (option != nullptr) {
        option->OnReadSuccess(obj);
      }
    }

    virtual void InvokeFailed() override {
      ReaderOption *option = _reader->GetReaderOption();
      if (option != nullptr) {
        option->OnReadFailed();
      }
    }

  private:
    R *_reader;

    friend class ResourceManager;
  };

  using MultMapReaders = std::multimap<ReaderId, ResourceReaderPtr>;
#if defined(_WIN32)
  using UnorderedCacheMap = std::unordered_map<CacheId, ResourceCache::Ptr>;
#elif defined(__linux__) or defined(__APPLE__)
  using UnorderedCacheMap = std::map<CacheId, ResourceCache::Ptr>;
#endif
  using MultiCacheMap = std::multimap<CacheId, ResourceCache::Ptr>;
  using WorkerPool = ThreadPool;
  using LoadingTaskSet = std::set<ReadTaskId>;
  using WaitToLoadTaskMap = std::multimap<ReadTaskId, IPackedReadTask::Ptr>;

  TSQueue<IPackedReadTask::Ptr> _taskQueue;
  TSQueue<IPackedReadTask::Ptr> _cachedTaskQueue;
  LoadingTaskSet _loadingTasks;
  WaitToLoadTaskMap _waitToLoadTasks;

  MultiCacheMap _sceneObjectsCacheMap;
  UnorderedCacheMap _objectCacheMap;

  MultMapReaders _loadingReaders;
  MultMapReaders _idleReaders;

  std::mutex _taskMutex;
  std::mutex _readerMutex;
  std::mutex _cacheMutex;

  WorkerPool _workerPool;

  friend class Singleton<ResourceManager>;
};
} // namespace TwinkleGraphics

#endif
