#ifndef TW_BOUNDINGVOLUME_H
#define TW_BOUNDINGVOLUME_H

#include "twFrustum.h"

namespace TwinkleGraphics
{
    class AABoundingBox;
    class OrientedBoundingBox;
    class BoundingSphere;

    class __TWCOMExport AABoundingBox : public Object
    {
    public:
        typedef std::shared_ptr<AABoundingBox> Ptr;

        AABoundingBox(vec3 min, vec3 max);
        AABoundingBox(const AABoundingBox &other);
        ~AABoundingBox();

        vec3 GetCenterPosition() { return (_min + _max) * 0.5f; }

        bool Intersect(const AABoundingBox &other);
        bool Intersect(const BoundingSphere &other);
        bool Intersect(const OrientedBoundingBox &other);
        bool Intersect(const Frustum &other);

    private:
        vec3 _min;
        vec3 _max;

        friend class OrientedBoundingBox;
        friend class BoundingSphere;
        friend class Frustum;
    };

    class __TWCOMExport OrientedBoundingBox : public Object
    {
    public:
        typedef std::shared_ptr<OrientedBoundingBox> Ptr;

        OrientedBoundingBox(vec3 min, vec3 max);
        OrientedBoundingBox(const OrientedBoundingBox &other);
        ~OrientedBoundingBox();

        vec3 GetCenterPosition() { return (_min + _max) * 0.5f; }

        bool Intersect(const AABoundingBox &other);
        bool Intersect(const BoundingSphere &other);
        bool Intersect(const OrientedBoundingBox &other);
        bool Intersect(const Frustum &other);

    private:
        vec3 _min;
        vec3 _max;

        friend class AABoundingBox;
        friend class BoundingSphere;
        friend class Frustum;
    };

    class __TWCOMExport BoundingSphere : public Object
    {
    public:
        typedef std::shared_ptr<BoundingSphere> Ptr;

        BoundingSphere(vec3 center, float radius);
        BoundingSphere(const BoundingSphere &other);
        ~BoundingSphere();

        vec3 GetCenterPosition() { return _center; }

        bool Intersect(const AABoundingBox &other);
        bool Intersect(const BoundingSphere &other);
        bool Intersect(const OrientedBoundingBox &other);
        bool Intersect(const Frustum &other);

    private:
        vec3 _center;
        float _radius;

        friend class OrientedBoundingBox;
        friend class AABoundingBox;
        friend class Frustum;
    };

} // namespace TwinkleGraphics

#endif
