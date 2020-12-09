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

        AABoundingBox(const vec3& min, const vec3& max);
        AABoundingBox(const AABoundingBox &other);
        virtual ~AABoundingBox();

        vec3 GetSize() { return _max - _min; }
        vec3 GetCorner(int index);
        vec3 GetCenter() { return (_min + _max) * 0.5f; }
        void ExpandByPoint(const vec3& point);
        void ExpandByBox(const AABoundingBox& box);
        void ExpandByScale(const vec3& scale);
        bool ContainPoint(const vec3& point);
        void MakeEmpty();
        bool IsEmpty();

        bool Intersect(const AABoundingBox &other);
        bool Intersect(const BoundingSphere &other);
        bool Intersect(const OrientedBoundingBox &other);
        bool Intersect(const Frustum &other);
        bool Intersect(const vec3& origin, const vec3& dir, float tMin = 0.0f, float tMax = std::numeric_limits<float>::max());
        bool Intersect(const vec3& planeNormal, float distance);

    private:
        bool IntersectRay(const vec3& origin, const vec3& dir);
        bool IntersectLine(const vec3& origin, const vec3& dir);
        bool IntersectLineSegment(const vec3& origin, const vec3& dir, float tMin, float tMax);

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

        OrientedBoundingBox(const vec3& min, const vec3& max);
        OrientedBoundingBox(const OrientedBoundingBox &other);
        virtual ~OrientedBoundingBox();

        vec3 GetCorner(int index);
        vec3 GetCenter() { return (_min + _max) * 0.5f; }

        bool Intersect(const AABoundingBox &other);
        bool Intersect(const BoundingSphere &other);
        bool Intersect(const OrientedBoundingBox &other);
        bool Intersect(const Frustum &other);
        bool Intersect(const vec3& origin, const vec3& dir, float tMin, float tMax);

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

        BoundingSphere(const vec3 center, float radius);
        BoundingSphere(const BoundingSphere &other);
        virtual ~BoundingSphere();

        vec3 GetCenter() { return _center; }

        bool Intersect(const AABoundingBox &other);
        bool Intersect(const BoundingSphere &other);
        bool Intersect(const OrientedBoundingBox &other);
        bool Intersect(const Frustum &other);
        bool Intersect(const vec3& origin, const vec3& dir, float tMin, float tMax);

    private:
        vec3 _center;
        float _radius;

        friend class OrientedBoundingBox;
        friend class AABoundingBox;
        friend class Frustum;
    };

} // namespace TwinkleGraphics

#endif
