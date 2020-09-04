#ifndef TW_THREADPOOL_H
#define TW_THREADPOOL_H


#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>

#include "twConsoleLog.h"

namespace TwinkleGraphics
{
    typedef uint32_t uint;
    typedef std::function<void()> Task;
    typedef std::shared_ptr<Task> TaskPtr;
    typedef std::thread Worker;

    template <typename T>
    bool TaskFinished(std::future<T> const &future)
    {
        using namespace std::chrono_literals;
        return future.wait_for(std::chrono::microseconds(0ms)) == std::future_status::ready;
    }

    template <class T>
    class TSQueue final
    {
    public:
        TSQueue() {}
        TSQueue(const TSQueue& src)
        {
            std::lock_guard<std::mutex> lock(src._mutex);
            _queue = src._queue;
        }

        void Push(const T& value)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _queue.emplace(value);
        }

        bool Pop(T& outValue)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if(_queue.empty())
            {
                return false;
            }

            outValue = std::move(_queue.front());
            _queue.pop();
            return true;
        }

        bool Empty()
        {
            std::lock_guard<std::mutex> lock(_mutex);
            return _queue.empty();
        }

        bool EmptyNoLock()
        {
            return _queue.empty();
        }

    private:
        std::queue<T> _queue;
        std::mutex _mutex;
    };

    class WorkThread final
    {
        public:
            WorkThread() {}
            ~WorkThread() {}

        private:
    };

    class ThreadPool final
    {
    public:
        ThreadPool(uint size);
        ~ThreadPool();

        template <class Func, class... Args>
        auto PushTask(Func &&f, Args &&... args)
            -> std::future<typename std::result_of<Func(Args...)>::type>;

        void Stop(bool delay = true);
        uint Size() { return static_cast<uint>(_workers.size()); }
        uint IdleCount() { return _idleCount.load(); }

    private:
        ThreadPool(const ThreadPool&) = delete;
        ThreadPool(ThreadPool&&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete;
        ThreadPool& operator=(ThreadPool&&) = delete;

        void Clear() 
        { 
            Task task;
            while(_tasks.Pop(task));
        }

    private:
        TSQueue<Task> _tasks;
        std::vector<Worker> _workers;
        mutable std::mutex _mutex;
        std::condition_variable _condition;

        std::atomic_uint _idleCount{0};
        std::atomic_bool _stoped{false};
    };

inline ThreadPool::ThreadPool(uint size)
{
    for(uint i = 0; i < size; i++)
    {
        _workers.emplace_back(
            [this]() -> bool
            {
                for(;;)
                {
                    Task task;
                    {
                        std::unique_lock<std::mutex> lock(_mutex);

                        ++this->_idleCount;
                        this->_condition.wait(lock, [this]() 
                        {
                            return this->_stoped.load() || !this->_tasks.Empty();
                        });
                        --this->_idleCount;

                        if(this->_stoped.load() && this->_tasks.Empty())
                        {
                            return true;
                        }

                        this->_tasks.Pop(task);
                    }
                    task();
                }
            }
        );
    }
}

inline ThreadPool::~ThreadPool()
{
    Stop(true);
}

template <class Func, class... Args>
auto ThreadPool::PushTask(Func&& f, Args&&... args)
    -> std::future<typename std::result_of<Func(Args...)>::type>
{
    if(_stoped.load())
    {
        Console::LogWarning("ThreadPool: ThreadPool stoped, we can't push task in.\n");
        throw std::runtime_error("Push Task While ThreadPool Has Stoped.");
    }

    using ReturnType = typename std::result_of<Func(Args...)>::type;

    auto packedTask = std::make_shared<std::packaged_task<ReturnType()>>(
        std::bind(std::forward<Func>(f), std::forward<Args>(args)...));
    
    auto task = std::make_shared<Task>([packedTask]()
    {
        (*packedTask)();
    });

    std::future<ReturnType> future = packedTask->get_future();
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _tasks.Push(std::move(*task));
    }
    _condition.notify_one();

    return future;
}


inline void ThreadPool::Stop(bool delay)
{
    if (!_stoped.load())
    {
        _stoped.store(true);
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (!delay)
            {
                Clear();
            }
        }
        _condition.notify_all();
    }

    for (auto &worker : _workers)
    {
        if (worker.joinable())
        {
            worker.join();

            Console::LogInfo("ThreadPool: WorkThread---", worker.get_id(), " stoped.\n");
        }
    }

    _workers.clear();
}

} // namespace TwinkleGraphics



#endif