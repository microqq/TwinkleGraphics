
#ifndef TW_COMUTIL_H
#define TW_COMUTIL_H

#include "twCommon.h"

namespace TwinkleGraphics
{
    /**
     * @brief https://www.partow.net/programming/hashfunctions/#DJBHashFunction
     * 
     * @param str 
     * @param length 
     * @return unsigned int 
     */
    unsigned int DJBHash(const char *str, unsigned int length);

    /**
     * @brief 
     * 
     * @tparam T 
     * @param a 
     * @param b 
     */
    template <class T>
    void Swap(T &a, T &b)
    {
        a = a - b;
        b = a + b;
        a = b - a;
    }

    /**
     * @brief 
     * 
     * @param point 
     * @param normal: normalized
     * @param d 
     * @return float: return signed distance
     */
    float DistanceFromPointToPlane(const vec3& point, const vec3& normal, float d)
    {
        // line that pass through point(p): L(t) = p + n * t
        // float t = -(glm::dot(point, normal) + d) / glm::dot(normal, normal);
        // optimize: we use t = -t;
        float t = glm::dot(point, normal) + d;
        return t;
    }

    /**
     * @brief 
     * 
     * @param plane 
     */
    void NormalizePlane(vec4& plane)
    {
        vec3 n(plane.x, plane.y, plane.z);
        float mag = glm::length(n);

        plane = plane / mag;
    }


} // namespace TwinkleGraphics

#endif