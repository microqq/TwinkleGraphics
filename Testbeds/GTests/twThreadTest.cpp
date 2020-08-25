
#include <vector>
#include <thread>
#include <future>
#include <numeric>
#include <iostream>
#include <chrono> 

#include <gtest/gtest.h>

#include "twConsoleLog.h"
#include "twThreadPool.h"

using namespace TwinkleGraphics;

std::vector<int> intVector_;
std::mutex intVecMutex_;

void func1()
{
    std::lock_guard<std::mutex> lock(intVecMutex_);
    for(int i = 0; i < 20; i++)
    {
        int size = intVector_.size();
        intVector_.emplace_back(size);

        Console::LogGTestInfo("intVector index ", i, intVector_[i], "\n");
    }
} 

void func2()
{
    std::lock_guard<std::mutex> lock(intVecMutex_);
    for(int i = 0; i < 10; i++)
    {
        int size = intVector_.size();
        intVector_.emplace_back(size);

        Console::LogGTestInfo("intVector index ", i, intVector_[i], "\n");
    }
}

TEST(ThreadTests, PushTasks)
{
    ThreadPool pool(2);

    pool.PushTask(func1);
    // pool.PushTask(func2);


}