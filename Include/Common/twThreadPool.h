#ifndef TW_THREADPOOL_H
#define TW_THREADPOOL_H

#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace TwinkleGraphics
{
    typedef std::function<void()> Task;
    typedef std::thread Worker;

    class ThreadPool
    {
        public:
            ThreadPool(uint32_t size);
            ~ThreadPool();

            template <class Func, class ...Args>
            void AddTask(Func&& f, Args&&... args);

        private:
            std::queue<Task> _tasks;
            std::vector<Worker> _workers;
            mutable std::mutex _mutex;
            std::condition_variable _condition;

            std::atomic_bool _stop{false};
    };

inline ThreadPool::ThreadPool(uint32_t size)
{

}

template <class Func, class... Args>
void ThreadPool::AddTask(Func&& f, Args&&... args)
{

}

inline ThreadPool::~ThreadPool()
{
    for(auto& worker : _workers)
    {
        if(worker.joinable())
        {
            worker.join();
        }
    }
}

} // namespace TwinkleGraphics



#endif