
#include <gtest/gtest.h>

#include "twRingBuffer.h"
#include "twConsoleLog.h"

using namespace TwinkleGraphics;

TEST(RingbufferTests, PushElements)
{
    RingBuffer<int, 128U> ringInt;

    Console::LogWithColor<Console::Color::MAGENTA>("RingBuffer<int, 128U> size: ", sizeof(ringInt), "\n");
}