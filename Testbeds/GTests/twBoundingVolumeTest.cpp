
#include <functional>
#include <gtest/gtest.h>
#include <string>


#include "twBoundingVolume.h"
#include "twConsoleLog.h"


using namespace TwinkleGraphics;
using namespace std::chrono_literals;

TEST(BoundingVolumeTest, First) {
  vec3 min(0.0f, 0.0f, 0.0f), max(1.0f, 1.0f, 1.0f);
  AABoundingBox aabb1(min, max);
  AABoundingBox aabb2(min, max);

  bool ret0 = aabb1.Intersect(aabb2);
  Console::LogGTestInfo(
      "AABoundingBox aabb1 intersect with AABoundingBox aabb2:", ret0, "\n");

  vec3 center(0.0f, 0.0f, 0.0f);
  BoundingSphere bs1(center, 1.0f);
  bool ret1 = aabb1.Intersect(bs1);
  Console::LogGTestInfo(
      "AABoundingBox aabb1 intersect with BoundingSphere bs1:", ret1, "\n");
};
