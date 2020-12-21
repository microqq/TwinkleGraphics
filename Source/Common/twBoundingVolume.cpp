#include "twBoundingVolume.h"
#include "twConsoleLog.h"
#include "twComUtil.h"

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
    vec3 AABoundingBox::GetCorner(int index) const
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

    bool AABoundingBox::ContainPoint(const vec3 &point) const
    {
        return _max.x >= point.x && _min.x <= point.x &&
               _max.y >= point.y && _min.y <= point.y &&
               _max.z >= point.z && _min.z <= point.z;
    }

    void AABoundingBox::MarkEmpty()
    {
        float const infinity = std::numeric_limits<float>::max();
        _min = _max = vec3(infinity, infinity, infinity);
    }

    bool AABoundingBox::IsEmpty() const
    {
        return _max == _min;
    }

    bool AABoundingBox::Intersect(const AABoundingBox &other) const
    {
        // Console::LogAssert(0, "intersect error.");

        return _max.x >= other._min.x &&
               _max.y >= other._min.y &&
               _max.z >= other._min.z &&
               _min.x <= other._max.x &&
               _min.y <= other._max.y &&
               _min.z <= other._max.z;
    }

    bool AABoundingBox::Intersect(const BoundingSphere &other) const
    {
        vec3 o1 = GetCenter();
        vec3 o2 = other._center;
        vec3 n = (o2 - o1);
        float t = glm::length(n);
        n = glm::normalize(n);

        vec3 p = (t - other._radius) * n + o1;
        return ContainPoint(p);
    }

    bool AABoundingBox::Intersect(const OrientedBoundingBox &other) const
    {
        return false;
    }

    /**
     * @brief 
     * <<Real-Time Rendering 4th editon>> Chapter22: Kay and Kajiya’s slab method
     * @param origin 
     * @param dir 
     * @param tMin 
     * @param tMax 
     * @return true 
     * @return false 
     */
    bool AABoundingBox::Intersect(const vec3 &origin, const vec3 &dir, float& t, float tMin, float tMax) const
    {
        float const infinity = std::numeric_limits<float>::max();

        if (tMax == infinity)
        {
            //if ray, tMin == 0.0f, tMax == infinity
            if (tMin == 0.0f)
            {
                return IntersectRay(origin, dir, t);
            }

            //if line, tMin == -infinity, tMax == inifinity
            if (tMin == -infinity)
            {
                float t1 = 0.0f, t2 = 0.0f;
                bool ret = IntersectLine(origin, dir, t1, t2);
                if(t2 < 0.0f)
                    t = t2;
                else if(t1 > 0.0f)
                    t = t1;
                else
                {
                    if(-t1 > t2)
                        t = t2;
                    else
                        t = t1; 
                }

                return ret;
            }

            Console::LogAssert(tMin == 0.0f || tMin == -infinity, "Ray(or Line) tMin has a value error.", "\n");

            return false;
        }
        else
        {
            //if segment, tMin != (-infinity || inifinity), tMax != (-infinity || inifinity)
            Console::LogAssert(tMin != infinity, "LineSegment tMin has a value error.", "\n");
            Console::LogAssert(tMax != infinity, "LineSegment tMax has a value error.", "\n");
            Console::LogAssert(tMin >= 0.0f && tMax > tMin, "LineSegment tMax should greatre than tMin.", "\n");

            return IntersectLineSegment(origin, dir, tMin, tMax, t);
        }
    }

    /**
     * @brief 
     *  <<Real-Time Rendering 4th Rendering>> Chapter22: Plane and box intersection, Page 971.
     *  (1) "One way to determine whether a box intersects a plane is to insert all the vertices
            of the box into the plane equation"
        (2) "The idea behind both methods is that only two of the eight corners need to be
            inserted into the plane equation"
        (3) "Every box has four diagonals, formed by its corners. Taking the dot product of each diagonal’s direction with the
            plane’s normal, the largest value identifies the diagonal with these two furthest points.
            By testing just these two corners, the box as a whole is tested against a plane"
        (4) "Why is this equivalent to finding the maximum of the eight different half diagonals
            projections?"
     * @param planeNormal 
     * @param distance 
     * @return true 
     * @return false
     */
    bool AABoundingBox::Intersect(const vec3 &planeNormal, float d, Intersection& intersection) const
    {
        // Plane Equation: Dot((A, B, C), (x, y, z)) + D = 0;
        // Compute the maximum of the eight different half diagnoals projections.
        // How? These eight half diagonals are the combinations: gi = (±hx, ±hy, ±hz), and we want to compute gi · n for all eight i.
        vec3 halfDiagonal((_max - _min) * 0.5f);
        // Compute projection of half diagonals vector along with plane normal.
        float e = glm::dot(halfDiagonal, glm::abs(planeNormal));
        
        // Box center to plane Distance Equation: distance = -Dot(Center, Normal) + D) / Dot(Normal, Normal).
        // if Normal normalized, Dot(Normal, Normal) == 1. So, distance = -Dot(Center, Normal) - D.
        // optimise: we use distance = -distance
        float s = glm::dot((_max + _min) * 0.5f, planeNormal) + d;

        // <<Real-Time Rendering 4th Rendering>> Chapter22: Figure 22.18, "Assuming that the “outside” of the plane is the positive half-space"
        intersection = (s - e > 0.0f) ? OUTSIDE : (s + e < 0.0f) ? INSIDE : INTERSECTING;
        
        return intersection == INTERSECTING;
    }

    bool AABoundingBox::IntersectRay(const vec3 &origin, const vec3 &dir, float& t) const
    {
        float tMin, tMax;
        if(IntersectLine(origin, dir, tMin, tMax))
        {
            if(tMax < 0.0f)
                return false;

            t = tMin > 0.0f ? tMin : tMax;
            return true;
        }
    }

    bool AABoundingBox::IntersectLine(const vec3 &origin, const vec3 &dir, float& t1, float t2) const
    {
        // line equation: Ray = o + t * n;
        // line intersect with box at p: o.x + t * n.x = p.x;
        vec3 invDir = 1.0f / dir;
        float tMin = (_min.x - origin.x) * invDir.x;
        float tMax = (_max.x - origin.x) * invDir.x;
        if(tMin > tMax)
        {
            Swap(tMin, tMax);
        }

        float ty0 = (_min.y - origin.y) * invDir.y;
        float ty1 = (_max.y - origin.y) * invDir.y;
        if(ty0 > ty1)
        {
            Swap(ty0, ty1);
        }

        if(tMin > ty1 || tMax < ty0)
        {
            return false;
        }

        if(tMin < ty0)
        {
            tMin = ty0;
        }

        if(tMax > ty1)
        {
            tMax = ty1;
        }

        float tz0 = (_min.z - origin.z) * invDir.z;
        float tz1 = (_max.z - origin.z) * invDir.z;
        if(tz0 > tz1)
        {
            Swap(tz0, tz1);
        }

        if(tMin > tz1 || tMax < tz0)
        {
            return false;
        }

        if(tMin < tz0)
        {
            tMin = tz0;
        }

        if(tMax > tz1)
        {
            tMax = tz1;
        }

        t1 = tMin; t2 = tMax;

        return true;
    }

    bool AABoundingBox::IntersectLineSegment(const vec3 &origin, const vec3 &dir, float tMin, float tMax, float& t) const
    {
        float t1, t2;
        if(IntersectLine(origin, dir, t1, t2))
        {
            if(t2 < 0.0f)
                return false;

            if(t1 > tMin && t1 < tMax)
            {
                t = t1;
                return true;
            }
            else if(t2 > tMin && t2 < tMax)
            {
                t = t2;
                return true;
            }

            return false;
        }
    }





    OrientedBoundingBox::OrientedBoundingBox(const vec3& center, const std::array<vec3, 3>& axis, const vec3& extents)
        : Object()
        , _center(center)
        , _axis(axis)
        , _extents(extents)
    {
    }

    OrientedBoundingBox::OrientedBoundingBox(const OrientedBoundingBox &other)
        : Object(other)
    {
    }

    OrientedBoundingBox::~OrientedBoundingBox()
    {
    }

    bool OrientedBoundingBox::Intersect(const BoundingSphere &other) const
    {
        return false;
    }

    bool OrientedBoundingBox::Intersect(const OrientedBoundingBox &other) const
    {
        return false;
    }

    bool OrientedBoundingBox::Intersect(const vec3 &planeNormal, float d, Intersection& intersection) const
    {

    }


    bool OrientedBoundingBox::Intersect(const vec3 &origin, const vec3 &dir, float tMin, float tMax) const
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

    void BoundingSphere::ExpandByPoint(const vec3 &point)
    {
        if(ContainPoint(point))
            return;
        
        float dist = DistancePoint2Point(_center, point);
        _radius = dist;
    }

    bool BoundingSphere::ContainPoint(const vec3 &point) const
    {
        float squareDist = SquareDistancePoint2Point(_center, point);
        return squareDist < _radius * _radius;
    }

    bool BoundingSphere::Intersect(const BoundingSphere &other) const
    {
        float squareR = _radius + other._radius;
        squareR = squareR * squareR;

        return SquareDistancePoint2Point(_center, other._center) < squareR;
    }

    bool BoundingSphere::Intersect(const vec3 &normal, float d)
    {
        float dist = DistancePoint2Plane(_center, vec4(normal, d));
        return dist <= _radius;
    }

    bool BoundingSphere::Intersect(const vec3 &origin, const vec3 &dir, float tMin, float tMax) const
    {
        float const infinity = std::numeric_limits<float>::max();

        // line equation: Ray = o + t * n;
        vec3 intersectPoint;
        float squareDist = SquareDistancePoint2Line(_center, origin, dir, intersectPoint);
        float squareR = _radius * _radius;
        if(squareDist > squareR)
            return false;

        if (tMax == infinity)
        {
            Console::LogAssert(tMin == 0.0f || tMin == -infinity, "Ray(or Line) tMin has a value error.", "\n");

            //if ray, tMin == 0.0f, tMax == infinity
            if (tMin == 0.0f)
            {
                float t;
                if (dir.x != 0.0f)
                    t = (intersectPoint.x - origin.x) / dir.x;
                else if (dir.y != 0.0f)
                    t = (intersectPoint.y - origin.y) / dir.y;
                else
                    t = (intersectPoint.z - origin.z) / dir.z;

                return t >= 0.0f;
            }

            //if line, tMin == -infinity, tMax == inifinity
            return true;
        }
        else
        {
            //if segment, tMin != (-infinity || inifinity), tMax != (-infinity || inifinity)
            Console::LogAssert(tMin != infinity && tMax != infinity && tMin >= 0.0f && tMax > tMin, "LineSegment tMin/tMax has a value error.", "\n");
        }

        return true;
    }
} // namespace TwinkleGraphics
