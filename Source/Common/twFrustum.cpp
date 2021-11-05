
#include "twFrustum.h"
#include "twBoundingVolume.h"
#include "twComUtil.h"

namespace TwinkleGraphics {
Frustum::Frustum(glm::mat4 &matrix) : Object() { FromMatrix(matrix); }

Frustum::Frustum(const Frustum &other) : Object(other) {
  for (int i = 0; i < 6; i++) {
    _planes[i] = other._planes[i];
  }
}

Frustum::~Frustum() {}

/**
 * @brief <<Real-Time Rendering 4th Edtion>> Chapter 22: Frustum Extraction
 *  (1) To make the normal of the plane point outward from the frustum, the
 *      equation must be negated (as the original equation described
 *      the inside of the unit cube)
 *
 * @param matrix
 */
void Frustum::FromMatrix(glm::mat4 &matrix) {
  vec4 matRow[4];
  matRow[0] = glm::row(matrix, 0);
  matRow[1] = glm::row(matrix, 1);
  matRow[2] = glm::row(matrix, 2);
  matRow[3] = glm::row(matrix, 3);

  _planes[0] = -(matRow[3] + matRow[0]); // left
  _planes[1] = -(matRow[3] - matRow[0]); // right
  _planes[2] = -(matRow[3] + matRow[1]); // bottom
  _planes[3] = -(matRow[3] - matRow[1]); // top
  _planes[4] = -(matRow[3] + matRow[2]); // near
  _planes[5] = -(matRow[3] - matRow[2]); // far

  NormalizePlane(_planes[0]);
  NormalizePlane(_planes[1]);
  NormalizePlane(_planes[2]);
  NormalizePlane(_planes[3]);
  NormalizePlane(_planes[4]);
  NormalizePlane(_planes[5]);
}

bool Frustum::ContainPoint(const vec3 &point) const {
  vec3 n;
  float d;
  for (int i = 0; i < 6; i++) {
    n.x = _planes[i].x;
    n.y = _planes[i].y;
    n.z = _planes[i].z;
    d = _planes[i].w;

    float dist = glm::dot(n, point) + d;
    if (dist < 0.0f) {
      return false;
    }
  }

  return true;
}

/**
 * @brief <<Real-Time Rendering 4th Edtion>> Chapter 22:
 *
 http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-testing-boxes-ii/
 *
 *  (1) Clippling test("Its self-contained simplicity
        lends it to efficient use in a compute shader")
 *  (2) Plane/Box intersection: find the most positive/negative corners.
 *
 *  it's inaccurate frustum/aabb intersection,
 *  eg:
 https://www.iquilezles.org/www/articles/frustumcorrect/frustumcorrect.htm
 *
 * @param other
 * @param intersection
 * @return true
 * @return false
 */
bool Frustum::Intersect(const AABoundingBox &other,
                        Intersection &intersection) const {
  for (int i = 0; i < 6; i++) {
    //
    float positive =
        glm::max(_planes[i].x * other._min.x, _planes[i].x * other._max.x) +
        glm::max(_planes[i].y * other._min.y, _planes[i].y * other._max.y) +
        glm::max(_planes[i].z * other._min.z, _planes[i].z * other._max.z) +
        _planes[i].w;

    if (positive > 0.0f) {
      intersection = INSIDE;
      continue;
    }

    float negative =
        glm::min(_planes[i].x * other._min.x, _planes[i].x * other._max.x) +
        glm::min(_planes[i].y * other._min.y, _planes[i].y * other._max.y) +
        glm::min(_planes[i].z * other._min.z, _planes[i].z * other._max.z) +
        _planes[i].w;

    if (negative > 0.0f) {
      intersection = OUTSIDE;
      return false;
    }

    intersection = INTERSECTING;
  }

  return true;
}

bool Frustum::Intersect(const BoundingSphere &other,
                        Intersection &intersection) const {
  vec3 center = other._center;
  float radius = other._radius;
  for (int i = 0; i < 6; i++) {
    vec3 n(_planes[i].x, _planes[i].y, _planes[i].z);
    float d = _planes[i].w;

    float dist = DistancePoint2Plane(center, n, d);
    if (dist > 0.0f && dist > radius) {
      intersection = OUTSIDE;
      return false;
    }

    if (dist < 0.0f && -dist >= radius) {
      intersection = INSIDE;
      continue;
    }

    intersection = INTERSECTING;
  }

  return true;
}

/**
 * @brief <<Real-Time Rendering 4th Edtion>> Chapter 22.
 *  http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-testing-boxes-ii/
 *
 *  (1) Clippling test
 *  (2) Plane/Box intersection: find the most positive/negative corners.
 *
 *
 * @param other
 * @return true
 * @return false
 */
bool Frustum::Intersect(const OrientedBoundingBox &other,
                        Intersection &intersection) const {
  for (int i = 0; i < 6; i++) {
    vec3 planeNormal(_planes[i].x, _planes[i].y, _planes[i].z);
    float projX = glm::dot(planeNormal, other._axis[0]);
    float projY = glm::dot(planeNormal, other._axis[1]);
    float projZ = glm::dot(planeNormal, other._axis[2]);

    vec3 negativePoint = other._extents, positivePoint = -other._extents;
    if (projX > 0.0f) {
      negativePoint.x = -other._extents.x;
      positivePoint.x = other._extents.x;
    }

    if (projY > 0.0f) {
      negativePoint.y = -other._extents.y;
      positivePoint.y = other._extents.y;
    }

    if (projZ > 0.0f) {
      negativePoint.z = -other._extents.z;
      positivePoint.z = other._extents.z;
    }

    negativePoint += other._center;
    positivePoint += other._center;

    float positive = DistancePoint2Plane(positivePoint, _planes[i]);
    if (positive < 0.0f) {
      intersection = INSIDE;
      continue;
    }

    float negative = DistancePoint2Plane(negativePoint, _planes[i]);
    if (negative < 0.0f) {
      intersection = OUTSIDE;
      return false;
    }

    intersection = INTERSECTING;
  }

  return true;
}

bool Frustum::Intersect(const vec3 &origin, const vec3 &dir, float &t,
                        float tMin, float tMax) const {
  return false;
}

bool Frustum::Intersect(const vec3 &planeNormal, float d,
                        Intersection &intersection) const {
  return false;
}

#pragma region Privates
float Frustum::GetMostPositive(const vec3 &p1, const vec3 &p2,
                               const vec4 &plane) {
  float positive = glm::max(plane.x * p1.x, plane.x * p2.x) +
                   glm::max(plane.y * p1.y, plane.y * p2.y) +
                   glm::max(plane.z * p1.z, plane.z * p2.z) + plane.w;

  return positive;
}

float Frustum::GetMostNegative(const vec3 &p1, const vec3 &p2,
                               const vec4 &plane) {
  float negative = glm::min(plane.x * p1.x, plane.x * p2.x) +
                   glm::min(plane.y * p1.y, plane.y * p2.y) +
                   glm::min(plane.z * p1.z, plane.z * p2.z) + plane.w;

  return negative;
}
#pragma endregion Privates

}; // namespace TwinkleGraphics