#include "twBoundingVolume.h"

namespace TwinkleGraphics
{
    AABoundingBox::AABoundingBox(const vec3& min, const vec3& max)
        : Object()
        , _min(min)
        , _max(max)
    {}

    AABoundingBox::AABoundingBox(const AABoundingBox& other)
        : Object(other)
        , _min(other._min)
        , _max(other._max)
    {}

    AABoundingBox::~AABoundingBox()
    {}

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
                    (index & 4) ? _max.z : _min.z
        );
    }

    void AABoundingBox::ExpandByPoint(const vec3 &point)
    {
        if(_max.x < point.x) _max.x = point.x;
        if(_max.y < point.y) _max.y = point.y;
        if(_max.z < point.z) _max.z = point.z;

        if(_min.x > point.x) _min.x = point.x;
        if(_min.y > point.y) _min.y = point.y;
        if(_min.z > point.z) _min.z = point.z;
    }

    void AABoundingBox::ExpandByBox(const AABoundingBox &box)
    {
        if(_max.x < box._max.x) _max.x = box._max.x;
        if(_max.y < box._max.y) _max.y = box._max.y;
        if(_max.z < box._max.z) _max.z = box._max.z;

        if(_min.x > box._min.x) _min.x = box._min.x;
        if(_min.y > box._min.y) _min.y = box._min.y;
        if(_min.z > box._min.z) _min.z = box._min.z;
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

    bool AABoundingBox::Intersect(const AABoundingBox &other)
    {
        return _max.x >= other._min.x &&
            _max.y >= other._min.y &&
            _max.z >= other._min.z &&
            _min.x <= other._max.x &&
            _min.y <= other._max.y &&
            _min.z <= other._max.z;
    }

    bool AABoundingBox::Intersect(const BoundingSphere &other)
    {
        
    }

    bool AABoundingBox::Intersect(const OrientedBoundingBox &other)
    {
        return false;
    }

    bool AABoundingBox::Intersect(const Frustum &other)
    {
        return false;
    }

    bool AABoundingBox::Intersect(const vec3& rayOrigin, const vec3& rayNormal, float delta)
    {
        return false;
    }

    bool AABoundingBox::Intersect(const vec3 &planeNormal, float distance)
    {
        return false;
    }



    OrientedBoundingBox::OrientedBoundingBox(const vec3& min, const vec3& max)
        : Object()
        , _min(min)
        , _max(max)
    {}

    OrientedBoundingBox::OrientedBoundingBox(const OrientedBoundingBox& other)
        : Object(other)
        , _min(other._min)
        , _max(other._max)
    {}

    OrientedBoundingBox::~OrientedBoundingBox()
    {}

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

    bool OrientedBoundingBox::Intersect(const vec3& rayOrigin, const vec3& rayNormal, float delta)
    {
        return false;
    }





    BoundingSphere::BoundingSphere(vec3 center, float radius)
        : Object()
        , _center(center)
        , _radius(radius)
    {}

    BoundingSphere::BoundingSphere(const BoundingSphere& other)
        : Object(other)
        , _center(other._center)
        , _radius(other._radius)
    {}

    BoundingSphere::~BoundingSphere()
    {}

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

    bool BoundingSphere::Intersect(const vec3& rayOrigin, const vec3& rayNormal, float delta)
    {
        return false;
    }



} // namespace TwinkleGraphics
