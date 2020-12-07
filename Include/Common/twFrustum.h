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

        Frustum();
        Frustum(const Frustum &other);
        ~Frustum();

        bool Intersect(const AABoundingBox &other);
        bool Intersect(const BoundingSphere &other);
        bool Intersect(const OrientedBoundingBox &other);
        bool Intersect(const Frustum &other);

    private:


        friend class OrientedBoundingBox;
        friend class BoundingSphere;
        friend class AABoundingBox;
    };
} // namespace TwinkleGraphics

#endif