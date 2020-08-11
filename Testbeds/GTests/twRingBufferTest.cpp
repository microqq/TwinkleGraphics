
#include <gtest/gtest.h>


#include "twRingBuffer.h"
#include "twConsoleLog.h"
#include "twEvent.h"

using namespace TwinkleGraphics;

TEST(RingbufferTests, PushPopElements)
{
    RingBuffer<int, 128U> ringInt;
    for(int i = 0; i < 20; i++)
    {
        int *elePtr = ringInt.PushBack();
        int &eleRef = *elePtr;
        eleRef = i;
    }

    ASSERT_EQ(ringInt[10] == 10, true);
    Console::LogGTestInfo("RingBuffer<int, 128U> [10]: ", ringInt[10], "\n");
    Console::LogGTestInfo("RingBuffer<int, 128U> size: ", sizeof(ringInt), "\n");

    RingBuffer<Event, 1> ringEvent;
    Event* event = ringEvent.PushBack();
    Event* event1 = ringEvent.PushBack();

    ASSERT_EQ(&(ringEvent[0]) == event, true);
    ASSERT_EQ(event1 == nullptr, true);
    Console::LogGTestInfo("RingBuffer<Event, 1> size: ", sizeof(ringEvent), "\n");
    Console::LogGTestInfo("RingBuffer<Event, 1> length: ", ringEvent.Length(), "\n");

    RingBuffer<Event, 10> ringEvent10;
    for(int i = 0; i < 20; i++)
    {
        ringEvent10.PushBack();
    }
    Console::LogGTestInfo("RingBuffer<Event, 10> push 20 elements\n");
    Console::LogGTestInfo("RingBuffer<Event, 10> length: ", ringEvent10.Length(), "\n");

    for(int i = 0; i < 5; i++)
    {
        ringEvent10.PopFront();
    }
    ASSERT_EQ(ringEvent10.Length() == 5, true);    
    Console::LogGTestInfo("RingBuffer<Event, 10> pop 5 elements\n");
    Console::LogGTestInfo("RingBuffer<Event, 10> length: ", ringEvent10.Length(), "\n");

    for (int i = 0; i < 3; i++)
    {
        ringEvent10.PushBack();
    }
    Console::LogGTestInfo("RingBuffer<Event, 10> push 3 elements\n");
    Console::LogGTestInfo("RingBuffer<Event, 10> length: ", ringEvent10.Length(), "\n");

    Console::LogGTestInfo("RingBuffer<Event, 10> head index: ", ringEvent10.HeadIndex(), "\n");
    Console::LogGTestInfo("RingBuffer<Event, 10> tail index: ", ringEvent10.TailIndex(), "\n");
}
