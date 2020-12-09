#include "twBoundingVolume.h"
#include "twConsoleLog.h"

namespace TwinkleGraphics
{
    AABoundingBox::AABoundingBox(const vec3 &min, const vec3 &max)
        : Object(), _min(min), _max(max)
    {
    }

    AABoundingBox::AABoundingBox(const AABoundingBox &other)
        : Object(other), _min(other._min), _max(other._max)
    {
    }

    AABoundingBox::~AABoundingBox()
    {
    }

    /**
     * @brief 
     * 3D Math Primer for Graphics and Game Development Chapter 12
     * 
     * @param index 
     * @return vec3 
     */
    vec3 AABoundingBox::GetCorner(int index)
    {
        assert(index >= 0 && index <= 7);

        return vec3((index & 1) ? _max.x : _min.x,
                    (index & 2) ? _max.y : _min.y,
                    (index & 4) ? _max.z : _min.z);
    }

    void AABoundingBox::ExpandByPoint(const vec3 &point)
    {
        if (_max.x < point.x)
            _max.x = point.x;
        if (_max.y < point.y)
            _max.y = point.y;
        if (_max.z < point.z)
            _max.z = point.z;

        if (_min.x > point.x)
            _min.x = point.x;
        if (_min.y > point.y)
            _min.y = point.y;
        if (_min.z > point.z)
            _min.z = point.z;
    }

    void AABoundingBox::ExpandByBox(const AABoundingBox &box)
    {
        if (_max.x < box._max.x)
            _max.x = box._max.x;
        if (_max.y < box._max.y)
            _max.y = box._max.y;
        if (_max.z < box._max.z)
            _max.z = box._max.z;

        if (_min.x > box._min.x)
            _min.x = box._min.x;
        if (_min.y > box._min.y)
            _min.y = box._min.y;
        if (_min.z > box._min.z)
            _min.z = box._min.z;
    }

    void AABoundingBox::ExpandByScale(const vec3 &scale)
    {
        vec3 size = GetSize();
        vec3 expandedSize = size * scale * 0.5f;
        vec3 center = GetCenter();
        _min = center - expandedSize;
        _max = center + expandedSize;
    }

    bool AABoundingBox::ContainPoint(const vec3 &point)
    {
        return _max.x >= point.x && _min.x <= point.x &&
               _max.y >= point.y && _min.y <= point.y &&
               _max.z >= point.z && _min.z <= point.z;
    }

    void AABoundingBox::MakeEmpty()
    {
        float const infinity = std::numeric_limits<float>::max();
        _min = _max = vec3(infinity, infinity, infinity);
    }

    bool AABoundingBox::IsEmpty()
    {
        return _max == _min;
    }

    bool AABoundingBox::Intersect(const AABoundingBox &other)
    {
        // Console::LogAssert(0, "intersect error.");

        return _max.x >= other._min.x &&
               _max.y >= other._min.y &&
               _max.z >= other._min.z &&
               _min.x <= other._max.x &&
               _min.y <= other._max.y &&
               _min.z <= other._max.z;
    }

    bool AABoundingBox::Intersect(const BoundingSphere &other)
    {
        vec3 o1 = GetCenter();
        vec3 o2 = other._center;
        vec3 n = (o2 - o1);
        float t = glm::length(n);
        n = glm::normalize(n);

        vec3 p = (t - other._radius) * n + o1;
        return ContainPoint(p);
    }

    bool AABoundingBox::Intersect(const OrientedBoundingBox &other)
    {
        return false;
    }

    bool AABoundingBox::Intersect(const Frustum &other)
    {
        return false;
    }

    /**
     * @brief 
     * Kay and Kajiya’s slab method
     * @param origin 
     * @param dir 
     * @param tMin 
     * @param tMax 
     * @return true 
     * @return false 
     */
    bool AABoundingBox::Intersect(const vec3 &origin, const vec3 &dir, float tMin, float tMax)
    {
        float const infinity = std::numeric_limits<float>::max();

        if (tMax == infinity)
        {
            //if ray, tMin == 0.0f, tMax == infinity
            if (tMin == 0.0f)
            {
                return IntersectRay(origin, dir);
            }

            //if line, tMin == -infinity, tMax == inifinity
            if (tMin == -infinity)
            {
                return IntersectLine(origin, dir);
            }

            Console::LogAssert(tMin == 0.0f || tMin == -infinity, "Ray(Line) tMin has a value error.", "\n");

            return false;
        }
        else
        {
            //if segment tMin != (-infinity || inifinity), tMax != (-infinity || inifinity)
            Console::LogAssert((tMin != -infinity) || (tMin != infinity), "Segment tMin has a value error.", "\n");

            Console::LogAssert((tMax == -infinity) || (tMax == infinity), "Segment tMax has a value error.", "\n");

            return IntersectLineSegment(origin, dir, tMin, tMax);
        }
    }

    bool AABoundingBox::Intersect(const vec3 &planeNormal, float distance)
    {
        return false;
    }

    bool AABoundingBox::IntersectRay(const vec3 &origin, const vec3 &dir)
    {
        // ray equation: Ray = o + t * n;(t > 0)
        // ray intersect with box at p: o.x + t * n.x = p.x;
        vec3 invDir = 1.0f / dir;
        float tMin, tMax;
        float tx0 = (_min.x - origin.x) * invDir.x;
        float tx1 = (_max.x - origin.x) * invDir.x;
        bool greater = tx0 > tx1;
        tMin = greater ? tx1 : tx0;
        tMax = greater ? tx0 : tx1;

        float ty0 = (_min.y - origin.y) * invDir.y;
        float ty1 = (_max.y - origin.y) * invDir.y;
        greater = ty0 > ty1;
        tMin = greater ? (tMin < ty1 ? ty1 : tMin) : (tMin < ty0 ? ty0 : tMin);
        tMax = greater ? (tMax > ty0 ? ty0 : tMax) : (tMax > ty1 ? ty1 : tMax);

        float tz0 = (_min.z - origin.z) * invDir.z;
        float tz1 = (_max.z - origin.z) * invDir.z;
        greater = tz0 > tz1;
        tMin = greater ? (tMin < tz1 ? tz1 : tMin) : (tMin < tz0 ? tz0 : tMin);
        tMax = greater ? (tMax > tz0 ? tz0 : tMax) : (tMax > tz1 ? tz1 : tMax);

        return tMin < tMax;
    }

    bool AABoundingBox::IntersectLine(const vec3 &origin, const vec3 &dir)
    {
    }

    bool AABoundingBox::IntersectLineSegment(const vec3 &origin, const vec3 &dir, float tMin, float tMax)
    {
    }





    OrientedBoundingBox::OrientedBoundingBox(const vec3 &min, const vec3 &max)
        : Object(), _min(min), _max(max)
    {
    }

    OrientedBoundingBox::OrientedBoundingBox(const OrientedBoundingBox &other)
        : Object(other), _min(other._min), _max(other._max)
    {
    }

    OrientedBoundingBox::~OrientedBoundingBox()
    {
    }

    vec3 OrientedBoundingBox::GetCorner(int index)
    {
    }

    bool OrientedBoundingBox::Intersect(const AABoundingBox &other)
    {
        return false;
    }

    bool OrientedBoundingBox::Intersect(const BoundingSphere &other)
    {
        return false;
    }

    bool OrientedBoundingBox::Intersect(const OrientedBoundingBox &other)
    {
        return false;
    }

    bool OrientedBoundingBox::Intersect(const Frustum &other)
    {
        return false;
    }

    bool OrientedBoundingBox::Intersect(const vec3 &origin, const vec3 &dir, float tMin, float tMax)
    {
        return false;
    }








    BoundingSphere::BoundingSphere(vec3 center, float radius)
        : Object(), _center(center), _radius(radius)
    {
    }

    BoundingSphere::BoundingSphere(const BoundingSphere &other)
        : Object(other), _center(other._center), _radius(other._radius)
    {
    }

    BoundingSphere::~BoundingSphere()
    {
    }

    bool BoundingSphere::Intersect(const AABoundingBox &other)
    {
        return false;
    }

    bool BoundingSphere::Intersect(const BoundingSphere &other)
    {
        return false;
    }

    bool BoundingSphere::Intersect(const OrientedBoundingBox &other)
    {
        return false;
    }

    bool BoundingSphere::Intersect(const Frustum &other)
    {
        return false;
    }

    bool BoundingSphere::Intersect(const vec3 &origin, const vec3 &dir, float tMin, float tMax)
    {
        return false;
    }

} // namespace TwinkleGraphics
