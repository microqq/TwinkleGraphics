
#include <gtest/gtest.h>


#include "twRingBuffer.h"
#include "twConsoleLog.h"
#include "twEvent.h"

using namespace TwinkleGraphics;

TEST(RingbufferTests, PushElements)
{
    RingBuffer<int, 128U> ringInt;
    int *buf = new (&(ringInt[10]))int(20);
    ASSERT_EQ(&(ringInt[10]) == buf, true);

    Console::LogGTestInfo("RingBuffer &(ringInt[10]) address ", &(ringInt[10]), "\n");
    Console::LogGTestInfo("RingBuffer buf address ", buf, "\n");
    Console::LogGTestInfo("RingBuffer<int, 128U> size: ", sizeof(ringInt), "\n");

    RingBuffer<Event, 1> ringEvent;
    Event* event = ringEvent.PushBack();
    Event* event1 = ringEvent.PushBack();

    ASSERT_EQ(&(ringEvent[0]) == event, true);
    ASSERT_EQ(event1 == nullptr, true);
    Console::LogGTestInfo("RingBuffer<Event> size: ", sizeof(ringEvent), "\n");
    Console::LogGTestInfo("RingBuffer<Event> length: ", ringEvent.Length(), "\n");
}