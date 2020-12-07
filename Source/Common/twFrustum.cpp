
#include "twFrustum.h"
#include "twBoundingVolume.h"

namespace TwinkleGraphics
{
    Frustum::Frustum()
        : Object()
    {}

    Frustum::Frustum(const Frustum& other)
        : Object(other)
    {}

    Frustum::~Frustum()
    {}

    bool Frustum::Intersect(const AABoundingBox &other)
    {
        return false;
    }

    bool Frustum::Intersect(const BoundingSphere &other)
    {
        return false;

    }

    bool Frustum::Intersect(const OrientedBoundingBox &other)
    {
        return false;

    }

    bool Frustum::Intersect(const Frustum &other)
    {
        return false;
    }
}; // namespace TwinkleGraphics