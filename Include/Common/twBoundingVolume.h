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

        vec3 GetSize() const { return _max - _min; }
        vec3 GetCorner(int index) const;
        vec3 GetCenter() const { return (_min + _max) * 0.5f; }
        void ExpandByPoint(const vec3& point);
        void ExpandByBox(const AABoundingBox& box);
        void ExpandByScale(const vec3& scale);
        bool ContainPoint(const vec3& point) const;
        void MarkEmpty();
        bool IsEmpty() const;

        bool Intersect(const AABoundingBox &other) const;
        bool Intersect(const BoundingSphere &other) const;
        bool Intersect(const OrientedBoundingBox &other) const;
        bool Intersect(const Frustum &other, Intersection& intersection) const;
        bool Intersect(const vec3& origin, const vec3& dir, float& t, float tMin = 0.0f, float tMax = std::numeric_limits<float>::max()) const;
        bool Intersect(const vec3& planeNormal, float d, Intersection& intersection) const;

    private:
        bool IntersectRay(const vec3& origin, const vec3& dir, float& t) const;
        bool IntersectLine(const vec3& origin, const vec3& dir, float& t1, float t2) const;
        bool IntersectLineSegment(const vec3& origin, const vec3& dir, float tMin, float tMax, float& t) const;

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

        OrientedBoundingBox();
        OrientedBoundingBox(const OrientedBoundingBox &other);
        virtual ~OrientedBoundingBox();

        vec3 GetCorner(int index) const;
        vec3 GetCenter() const { return _center; }
        vec3 GetSize() const { return _size; }
        glm::quat GetOrientation() const { return _orientation; }

        bool Intersect(const AABoundingBox &other) const;
        bool Intersect(const BoundingSphere &other) const;
        bool Intersect(const OrientedBoundingBox &other) const;
        bool Intersect(const Frustum &other) const;
        bool Intersect(const vec3& origin, const vec3& dir, float tMin, float tMax) const;

    private:
        vec3 _center;
        vec3 _size;
        glm::quat _orientation;

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

        vec3 GetCenter() const { return _center; }
        float GetRadius() const { return _radius; }
        void ExpandByPoint(const vec3& point);
        bool ContainPoint(const vec3& point) const;

        bool Intersect(const AABoundingBox &other) const;
        bool Intersect(const BoundingSphere &other) const;
        bool Intersect(const OrientedBoundingBox &other) const;
        bool Intersect(const Frustum &other, Intersection& intersection) const;
        bool Intersect(const vec3& origin, const vec3& dir, float tMin = 0.0f, float tMax = std::numeric_limits<float>::max()) const;

    private:
        vec3 _center;
        float _radius;

        friend class OrientedBoundingBox;
        friend class AABoundingBox;
        friend class Frustum;
    };

} // namespace TwinkleGraphics

#endif
