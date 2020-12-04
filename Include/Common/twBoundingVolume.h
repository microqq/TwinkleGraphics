#ifndef TW_BOUNDINGVOLUME_H
#define TW_BOUNDINGVOLUME_H

#include "twFrustum.h"

namespace TwinkleGraphics
{
    class AABoundingBox;
    class OrientedBoudingBox;
    class BoundingSphere;

    class AABoundingBox : public Object
    {
    public:
        typedef std::shared_ptr<AABoundingBox> Ptr;

        AABoundingBox();
        AABoundingBox(const AABoundingBox &other);
        ~AABoundingBox();

        bool Intersect(const AABoundingBox &other);
        bool Intersect(const BoundingSphere &other);
        bool Intersect(const OrientedBoudingBox &other);
        bool Intersect(const Frustum &other);
    };

    class OrientedBoudingBox : public Object
    {
    public:
        typedef std::shared_ptr<OrientedBoudingBox> Ptr;

        OrientedBoudingBox();
        OrientedBoudingBox(const OrientedBoudingBox &other);
        ~OrientedBoudingBox();

        bool Intersect(const AABoundingBox &other);
        bool Intersect(const BoundingSphere &other);
        bool Intersect(const OrientedBoudingBox &other);
        bool Intersect(const Frustum &other);
    };

    class BoundingSphere : public Object
    {
    public:
        typedef std::shared_ptr<BoundingSphere> Ptr;

        BoundingSphere();
        BoundingSphere(const BoundingSphere &other);
        ~BoundingSphere();

        bool Intersect(const AABoundingBox &other);
        bool Intersect(const BoundingSphere &other);
        bool Intersect(const OrientedBoudingBox &other);
        bool Intersect(const Frustum &other);
    };

    template <class T>
    class BoundingVolume
    {
    public:
        BoundingVolume(T& volume);
        BoundingVolume(const BoundingVolume &other);
        virtual ~BoundingVolume();

        bool Intersect();

    private:
        T _volume;
    };
} // namespace TwinkleGraphics

#endif
