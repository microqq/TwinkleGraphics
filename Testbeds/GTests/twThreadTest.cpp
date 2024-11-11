
#include <chrono>
#include <future>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>


#include <gtest/gtest.h>

#include "twConsoleLog.h"
#include "twShader.h"
#include "twThreadPool.h"


using namespace TwinkleGraphics;
using namespace std::chrono_literals;

std::vector<int> IntVector_;
std::mutex IntVecMutex_;
std::condition_variable IntVecCV_;
int ConsumerID_{0};
std::atomic_int ConsumedProductCount_{0};
int ProducerID_{0};
int ProductLimit_{20};
std::atomic_int CurrentProductCount_{0};

void Producer() {
  int id = ProducerID_++;
  std::unique_lock<std::mutex> lock(IntVecMutex_, std::defer_lock);
  // lock.unlock();
  while (true) {
    if (CurrentProductCount_.load() >= ProductLimit_) {
      std::this_thread::sleep_for(1000ms);
      continue;
    }

    lock.lock();
    {
      CurrentProductCount_++;
      IntVector_.emplace_back(IntVector_.size());
      Console::LogGTestInfo<Console::Color::GREEN, Console::Color::MAGENTA>(
          "Current Producer---", id,
          " current product count: ", CurrentProductCount_.load(), "\n");
    }
    lock.unlock();

    IntVecCV_.notify_one();
    std::this_thread::sleep_for(1000ms);
  }
}

void Consumer() {
  int id = ConsumerID_++;
  std::unique_lock<std::mutex> lock(IntVecMutex_, std::defer_lock);
  while (true) {
    std::this_thread::sleep_for(1200ms);
    lock.lock();
    {
      IntVecCV_.wait(lock, []() -> bool { return !IntVector_.empty(); });

      CurrentProductCount_--;
      ConsumedProductCount_++;
      IntVector_.pop_back();
      Console::LogGTestInfo(
          "Current Consumer---", id,
          " consumed product count: ", ConsumedProductCount_.load(), "\n");
    }
    lock.unlock();
  }
}

TEST(ThreadTests, PushTasks) {
  ThreadPool pool(2);

  pool.PushTask(Producer);
  pool.PushTask(Consumer);
};

TEST(ThreadTests, ProducerConsumers) {
  ThreadPool pool(4);

  pool.PushTask(Producer);
  pool.PushTask(Consumer);
  pool.PushTask(Consumer);
  pool.PushTask(Consumer);
};

TEST(ThreadTests, ProducersConsumer) {
  ThreadPool pool(4);

  pool.PushTask(Producer);
  pool.PushTask(Producer);
  pool.PushTask(Producer);
  pool.PushTask(Consumer);
};

TEST(ThreadTests, ProducersConsumers) {
  ThreadPool pool(6);

  pool.PushTask(Producer);
  pool.PushTask(Producer);
  pool.PushTask(Producer);
  pool.PushTask(Consumer);
  pool.PushTask(Consumer);
  pool.PushTask(Consumer);
};

template <typename T> struct Result {
  Result() {}
  Result(T &data) : _data(data) {}
  Result(const Result &src) : _data(src._data) {}
  Result &operator=(const Result &src) {
    _data = src._data;
    return *this;
  }

  T _data = 100;
};

Result<int> ReturnResult(int start) {
  int sum = start;
  for (int i = 0; i < 1000; i++) {
    sum += i;
  }

  return Result<int>(sum);
}

ReadResult<Shader> ReturnShader(int start) {
  int sum = start;
  for (int i = 0; i < 1000; i++) {
    sum += i;
  }

  ReadResult<Shader> result(ReadResult<Shader>::Status::FAILED);
  return result;
}

std::vector<std::future<Result<int>>> Futures_;

std::vector<std::future<ReadResult<Shader>>> FuturesOfShader_;

TEST(ThreadTests, FutureGet) {
  ThreadPool pool(3);

  for (int i = 0; i < 10; i++) {
    auto future = pool.PushTask(ReturnResult, i);
    Futures_.emplace_back(std::move(future));
  }

  // while(true)
  {
    for (auto &future : Futures_) {
      if (future.valid() && TaskFinished(future)) {
        Result<int> result = future.get();
        Console::LogGTestInfo("Future result<int>:", result._data, "\n");
      }
    }
  }

  pool.Stop(true);
};

TEST(ThreadTests, FutureGet2) {
  ThreadPool pool(3);

  for (int i = 0; i < 10; i++) {
    auto future = pool.PushTask(ReturnShader, i);
    FuturesOfShader_.emplace_back(std::move(future));
  }

  {
    for (auto &future : FuturesOfShader_) {
      if (future.valid() && TaskFinished(future)) {
        ReadResult<Shader> result = future.get();
        Console::LogGTestInfo("Future of shader: ................\n");
      }
    }
  }

  pool.Stop(true);
}