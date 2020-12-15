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
    enum Intersection;

    class __TWCOMExport Frustum : public Object
    {
    public:
        typedef std::shared_ptr<Frustum> Ptr;

        Frustum(glm::mat4& matrix);
        Frustum(const Frustum &other);
        ~Frustum();

        void FromMatrix(glm::mat4& matrix);
        bool Intersect(const AABoundingBox &other, Intersection& intersection);
        bool Intersect(const BoundingSphere &other, Intersection& intersection);
        bool Intersect(const OrientedBoundingBox &other);
        bool Intersect(const Frustum &other);

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