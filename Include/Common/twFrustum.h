#ifndef TW_FRUSTUM_H
#define TW_FRUSTUM_H

#include <memory>
#include "twCommon.h"
#include "twObject.h"

namespace TwinkleGraphics
{
    class AABoundingBox;
    class OrientedBoundingBox;
    class BoundingSphere;

    class __TWCOMExport Frustum : public Object
    {
    public:
        typedef std::shared_ptr<Frustum> Ptr;

        Frustum(glm::mat4& matrix);
        Frustum(const Frustum &other);
        ~Frustum();

        void FromMatrix(glm::mat4& matrix);
        bool Intersect(const AABoundingBox &other, Intersection& intersection) const;
        bool Intersect(const BoundingSphere &other, Intersection& intersection) const;
        bool Intersect(const OrientedBoundingBox &other) const;
        bool ContainPoint(const vec3& point) const;

    private:

    private:
        // plane: (nx, ny,nz, d);
        // 0-5: left, right, bottom, top, near, far
        vec4 _planes[6];

        friend class OrientedBoundingBox;
        friend class BoundingSphere;
        friend class AABoundingBox;
    };
} // namespace TwinkleGraphics

#endif