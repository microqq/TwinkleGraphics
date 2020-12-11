
#include "twFrustum.h"
#include "twBoundingVolume.h"

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

    void Frustum::FromMatrix(glm::mat4& matrix)
    {
        vec4 matRow[4];
        matRow[0] = glm::row(matrix, 0);
        matRow[1] = glm::row(matrix, 1);
        matRow[2] = glm::row(matrix, 2);
        matRow[3] = glm::row(matrix, 3);

        _planes[0] = matRow[3] + matRow[0]; //left
        _planes[1] = matRow[3] - matRow[0]; //right
        _planes[2] = matRow[3] + matRow[1]; //bottom
        _planes[3] = matRow[3] - matRow[1]; //top
        _planes[4] = matRow[3] + matRow[2]; //near
        _planes[5] = matRow[3] - matRow[2]; //far
    }

    bool Frustum::Intersect(const AABoundingBox &other)
    {
        return false;
    }

    bool Frustum::Intersect(const BoundingSphere &other)
    {
        return false;

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