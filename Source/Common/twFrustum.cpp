
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

    bool Frustum::Intersect(const AABoundingBox &other, Intersection& intersection)
    {
        return false;
    }

    bool Frustum::Intersect(const BoundingSphere &other, Intersection& intersection)
    {
        vec3 center = other._center;
        float radius = other._radius;
        for(int i = 0; i < 6; i++)
        {
            vec3 n(_planes[i].x, _planes[i].y, _planes[i].z);
            float d = _planes[i].w;

            float distance = DistanceFromPointToPlane(center, n, d);
            if(distance > radius)
            {
                intersection = OUTSIDE;
                return false;
            }

            if(-distance > radius)
            {
                intersection = INSIDE;
                continue;
            }

            intersection = INTERSECTING;
        }

        return true;
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