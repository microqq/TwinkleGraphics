
#include <functional>
#include <string>
#include <gtest/gtest.h>

#include "twConsoleLog.h"
#include "twBoundingVolume.h"

using namespace TwinkleGraphics;
using namespace std::chrono_literals;

TEST(BoundingVolumeTest, AABoundingBoxTest)
{
    AABoundingBox aabb1;
    AABoundingBox aabb2;

    bool ret0 = aabb1.Intersect(aabb2);
    Console::LogGTestInfo("AABoundingBox aabb1 intersect with AABoundingBox aabb2:", ret0, "\n");

    BoundingSphere bs1;
    bool ret1 = aabb1.Intersect(bs1);
    Console::LogGTestInfo("AABoundingBox aabb1 intersect with BoundingSphere bs1:", ret1, "\n");
};

TEST(ResourceReaderTests, BoundingSphereTest)
{
};

TEST(ResourceReaderTests, OrientedBoundingBoxTest)
{
};

TEST(ResourceReaderTests, FrustumTest)
{
};
