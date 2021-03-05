
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

    template <class T>
    void SwapNumber(T& a, T& b);


    /**
     * @brief 
     * 
     * @param point 
     * @param normal: normalized
     * @param d 
     * @return float: return signed distance
     */
    float DistancePoint2Plane(const vec3& point, const vec3& normal, float d);

    /**
     * @brief 
     * 
     * @param point 
     * @param plane 
     * @return float 
     */
    float DistancePoint2Plane(const vec3& point, const vec4& plane);

    /**
     * @brief 
     * 
     * @param point 
     * @param plane 
     * @return float 
     */
    float SquareDistancePoint2Plane(const vec3& point, const vec4& plane);


    /**
     * @brief 
     * 
     * @param p1 
     * @param p2 
     * @return float 
     */
    float DistancePoint2Point(const vec3& p1, const vec3& p2);

    /**
     * @brief 
     * 
     * @param p1 
     * @param p2 
     * @return float 
     */
    float SquareDistancePoint2Point(const vec3& p1, const vec3& p2);

    /**
     * @brief 
     * 
     * @param point 
     * @param origin 
     * @param dir: normalized direction
     * @param intersectPoint 
     * @return float 
     */
    float DistancePoint2Line(const vec3& point, const vec3& origin, const vec3& dir, vec3& intersectPoint);

    /**
     * @brief 
     * 
     * @param point 
     * @param origin 
     * @param dir 
     * @param intersectPoint 
     * @return float 
     */
    float SquareDistancePoint2Line(const vec3& point, const vec3& origin, const vec3& dir, vec3& intersectPoint);

    /**
     * @brief 
     * 
     * @param plane 
     */
    void NormalizePlane(vec4& plane);


} // namespace TwinkleGraphics

#endif