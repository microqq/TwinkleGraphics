
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
    Console::LogGTestInfo("Current Thread Id: ", std::this_thread::get_id(), "\n");

    int size = intVector_.size();
    for(int i = 0; i < 20; i++)
    {
        intVector_.emplace_back(size + i);
    }

    for(int i = size; i < size + 20; i++)
    {
        Console::LogGTestInfo("intVector index ", i, " value ", intVector_[i], "\n");
    }
} 

void func2()
{
    std::lock_guard<std::mutex> lock(intVecMutex_);
    Console::LogGTestInfo("Current Thread Id: ", std::this_thread::get_id(), "\n");

    int size = intVector_.size();
    for(int i = 0; i < 10; i++)
    {
        intVector_.emplace_back(size + i);
    }

    for(int i = size; i < size + 10; i++)
    {
        Console::LogGTestInfo("intVector index ", i, " value ", intVector_[i], "\n");
    }
}

TEST(ThreadTests, PushTasks)
{
    ThreadPool pool(4);

    pool.PushTask(func1);
    pool.PushTask(func2);
    pool.PushTask(func1);
}