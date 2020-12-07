#include "twBoundingVolume.h"

namespace TwinkleGraphics
{
    AABoundingBox::AABoundingBox(vec3 min, vec3 max)
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

    bool AABoundingBox::Intersect(const AABoundingBox &other)
    {
        return false;
    }

    bool AABoundingBox::Intersect(const BoundingSphere &other)
    {
        return false;
    }

    bool AABoundingBox::Intersect(const OrientedBoundingBox &other)
    {
        return false;
    }

    bool AABoundingBox::Intersect(const Frustum &other)
    {
        return false;
    }



    OrientedBoundingBox::OrientedBoundingBox(vec3 min, vec3 max)
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


} // namespace TwinkleGraphics
