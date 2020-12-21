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
        bool ContainPoint(const vec3& point) const;

        bool Intersect(const AABoundingBox &other, Intersection& intersection) const;
        bool Intersect(const BoundingSphere &other, Intersection& intersection) const;
        bool Intersect(const OrientedBoundingBox &other, Intersection& intersection) const;
        /**
         * @brief Intersect with ray/line/line segment
         * 
         * @param origin 
         * @param dir 
         * @param t 
         * @param tMin 
         * @param tMax 
         * @return true 
         * @return false 
         */
        bool Intersect(const vec3& origin, const vec3& dir, float& t, float tMin = 0.0f, float tMax = std::numeric_limits<float>::max()) const;
        /**
         * @brief Intersect with plane
         * 
         * @param planeNormal 
         * @param d 
         * @param intersection 
         * @return true 
         * @return false 
         */
        bool Intersect(const vec3& planeNormal, float d, Intersection& intersection) const;


    private:
        float GetMostPositive(const vec3& p1, const vec3& p2, const vec4& plane);
        float GetMostNegative(const vec3& p1, const vec3& p2, const vec4& plane);

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