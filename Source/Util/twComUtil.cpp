#include <random>
#include <algorithm>

#include "twComUtil.h"

namespace TwinkleGraphics {
std::string RandomString() {
  std::string str(
      "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

  std::random_device rd;
  std::mt19937 generator(rd());

  std::shuffle(str.begin(), str.end(), generator);

  return str.substr(0, 32); // assumes 32 < number of characters in str
}

#pragma region HashFunction
unsigned int DJBHash(const char *str, unsigned int length) {
  unsigned int hash = 5381;
  unsigned int i = 0;

  for (i = 0; i < length; ++str, ++i) {
    hash = ((hash << 5) + hash) + (*str);
  }

  return hash;
}
#pragma endregion HashFunction

template <> void SwapNumber(int &a, int &b) {
  a = a - b;
  b = a + b;
  a = (b - a) / 2;
}

float DistancePoint2Plane(const vec3 &point, const vec3 &normal, float d) {
  // line that pass through point(p): L(t) = p + n * t
  // float t = -(glm::dot(point, normal) + d) / glm::dot(normal, normal);
  // optimize: we use t = -t;
  float t = glm::dot(point, normal) + d;
  return t;
}

float DistancePoint2Plane(const vec3 &point, const vec4 &plane) {
  vec3 normal(plane.x, plane.y, plane.z);
  return DistancePoint2Plane(point, normal, plane.w);
}

float SquareDistancePoint2Plane(const vec3 &point, const vec4 &plane) {
  // line: L(t) = o + n * t;
  float dist = DistancePoint2Plane(point, plane);
  return dist * dist;
}

float DistancePoint2Point(const vec3 &p1, const vec3 &p2) {
  vec3 v = p2 - p1;
  return glm::length(v);
}

float SquareDistancePoint2Point(const vec3 &p1, const vec3 &p2) {
  vec3 v = p2 - p1;
  return glm::dot(v, v);
}

float DistancePoint2Line(const vec3 &point, const vec3 &origin, const vec3 &dir,
                         vec3 &intersectPoint) {
  // line: L(t) = origin + dir * t;
  // plane: Plane: Dot(dir,  p(x, y, z)) + d = 0; <==> (1)
  // Dot(dir, origin + dir * t) + d = 0;(1)
  // Dot(dir, point) + d = 0;(2)
  // Dot(dir, origin + dir * t) = Dot(dir, point);
  float t = glm::dot(dir, point) - glm::dot(dir, origin);
  intersectPoint = origin + dir * t;

  return DistancePoint2Point(point, intersectPoint);
}

float SquareDistancePoint2Line(const vec3 &point, const vec3 &origin,
                               const vec3 &dir, vec3 &intersectPoint) {
  // line: L(t) = origin + dir * t;
  // plane: Plane: Dot(dir,  p(x, y, z)) + d = 0; <==> (1)
  // Dot(dir, origin + dir * t) + d = 0;(1)
  // Dot(dir, point) + d = 0;(2)
  // Dot(dir, origin + dir * t) = Dot(dir, point);
  float t = glm::dot(dir, point) - glm::dot(dir, origin);
  intersectPoint = origin + dir * t;

  return SquareDistancePoint2Point(point, intersectPoint);
}

void NormalizePlane(vec4 &plane) {
  vec3 n(plane.x, plane.y, plane.z);
  float mag = glm::length(n);

  plane = plane / mag;
}

} // namespace TwinkleGraphics
