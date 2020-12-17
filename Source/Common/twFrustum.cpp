
#include "twFrustum.h"
#include "twBoundingVolume.h"
#include "twComUtil.h"

namespace TwinkleGraphics
{
    Frustum::Frustum(glm::mat4& matrix)
        : Object()
    {
        FromMatrix(matrix);
    }

    Frustum::Frustum(const Frustum& other)
        : Object(other)
    {
        for(int i = 0; i < 6; i++)
        {
            _planes[i] = other._planes[i];
        }
    }

    Frustum::~Frustum()
    {}

    /**
     * @brief <<Real-Time Rendering 4th Edtion>> Chapter 22: Frustum Extraction
     *  (1) To make the normal of the plane point outward from the frustum, the 
     *      equation must be negated (as the original equation described
     *      the inside of the unit cube)
     * 
     * @param matrix 
     */
    void Frustum::FromMatrix(glm::mat4& matrix)
    {
        vec4 matRow[4];
        matRow[0] = glm::row(matrix, 0);
        matRow[1] = glm::row(matrix, 1);
        matRow[2] = glm::row(matrix, 2);
        matRow[3] = glm::row(matrix, 3);

        _planes[0] = -(matRow[3] + matRow[0]); //left
        _planes[1] = -(matRow[3] - matRow[0]); //right
        _planes[2] = -(matRow[3] + matRow[1]); //bottom
        _planes[3] = -(matRow[3] - matRow[1]); //top
        _planes[4] = -(matRow[3] + matRow[2]); //near
        _planes[5] = -(matRow[3] - matRow[2]); //far

        NormalizePlane(_planes[0]);
        NormalizePlane(_planes[1]);
        NormalizePlane(_planes[2]);
        NormalizePlane(_planes[3]);
        NormalizePlane(_planes[4]);
        NormalizePlane(_planes[5]);
    }

    /**
     * @brief <<Real-Time Rendering 4th Edtion>> Chapter 22:
     *  (1) Clippling test("Its self-contained simplicity
            lends it to efficient use in a compute shader")
     *  (2) Plane/Box intersection: find nearest/furthest corners.
     *  
     *  it's inaccurate frustum/aabb intersection, 
     *  eg: https://www.iquilezles.org/www/articles/frustumcorrect/frustumcorrect.htm
     * 
     * @param other 
     * @param intersection 
     * @return true 
     * @return false 
     */
    bool Frustum::Intersect(const AABoundingBox &other, Intersection& intersection) const
    {
        for(int i = 0; i < 6; i++)
        {
            // 
            float furthest = glm::max(_planes[i].x * other._min.x, _planes[i].x * other._max.x) +
            glm::max(_planes[i].y * other._min.y, _planes[i].y * other._max.y) +
            glm::max(_planes[i].z * other._min.z, _planes[i].z * other._max.z) +
            _planes[i].w;

            if(furthest > 0.0f)
            {
                intersection = INSIDE;
                continue;
            }

            float nearest = glm::min(_planes[i].x * other._min.x, _planes[i].x * other._max.x) +
            glm::min(_planes[i].y * other._min.y, _planes[i].y * other._max.y) +
            glm::min(_planes[i].z * other._min.z, _planes[i].z * other._max.z) +
            _planes[i].w;

            if(nearest > 0.0f)
            {
                intersection = OUTSIDE;
                return false;
            }

            intersection = INTERSECTING;
        }

        return true;
    }

    bool Frustum::Intersect(const BoundingSphere &other, Intersection& intersection) const
    {
        vec3 center = other._center;
        float radius = other._radius;
        for(int i = 0; i < 6; i++)
        {
            vec3 n(_planes[i].x, _planes[i].y, _planes[i].z);
            float d = _planes[i].w;

            float distance = DistancePoint2Plane(center, n, d);
            if(distance > 0.0f && distance > radius)
            {
                intersection = OUTSIDE;
                return false;
            }

            if(distance < 0.0f && -distance >= radius)
            {
                intersection = INSIDE;
                continue;
            }

            intersection = INTERSECTING;
        }

        return true;
    }

    /**
     * @brief <<Real-Time Rendering 4th Edtion>> Chapter 22:
     *  (1) Clippling test
     *  (2) Plane/Box intersection: find nearest/furthest corners.
     * 
     * @param other 
     * @return true 
     * @return false 
     */
    bool Frustum::Intersect(const OrientedBoundingBox &other) const
    {
 
         return false;
    }

    bool Frustum::ContainPoint(const vec3 &point) const
    {
        vec3 n;
        float d;
        for(int i = 0; i < 6; i++)
        {
            n.x = _planes[i].x;
            n.y = _planes[i].y;
            n.z = _planes[i].z;
            d = _planes[i].w;

            float distance = glm::dot(n, point) + d;
            if(distance < 0.0f)
            {
                return false;
            }
        }

        return true;
    }
}; // namespace TwinkleGraphics