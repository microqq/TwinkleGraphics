

#ifndef TW_UTIL_H
#define TW_UTIL_H

#include "twCommon.h"
#include "twGeometry.h"
#include "twSprite.h"

namespace TwinkleGraphics
{
#ifdef __cplusplus
    extern "C"
    {
#endif
        __TWExport Triangle *CreateTriangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);

        __TWExport Line *CreateLine(glm::vec3 *points, int32 num);
        __TWExport Quad *CreateQuad(glm::vec2 size, MeshDataFlag flag = MeshDataFlag::DEFAULT);
        __TWExport Cube *CreateCube(float32 size, MeshDataFlag flag = MeshDataFlag::DEFAULT);
        __TWExport UVSphere *CreateUVSphere(float32 radius = 1.0f, int32 subdivision = 20, MeshDataFlag flag = MeshDataFlag::DEFAULT);
        __TWExport NormalizedCubeSphere *CreateNorCubeSphere(float32 radius = 1.0f, int32 subdivision = 20, MeshDataFlag flag = MeshDataFlag::DEFAULT);
        __TWExport IcosahedronSphere *CreateIcosahedronSphere(float32 radius = 1.0f, int32 subdivision = 20, MeshDataFlag flag = MeshDataFlag::DEFAULT);
        __TWExport BezierCurve *CreateBezierCurve(glm::vec4 *points, int32 num, int32 degree, int32 segments = 128);
        __TWExport BSplineCurve *CreateBSplineCurve(glm::vec4 *points, int32 num, Knot *knots, int32 knots_num, int32 degree, int32 segments = 128);
        __TWExport NURBSSurface *CreateNURBSSurface(glm::vec4 *points, Knot *uknots, Knot *vknots, int32 unum, int32 vnum, int32 udegree, int32 vdegree, int32 subdivide = 16, bool rational = false, MeshDataFlag flag = MeshDataFlag(13));

        __TWExport Plane *CreatePlane(glm::vec3 normal, float32 width, int32 subdivision = 32, MeshDataFlag flag = MeshDataFlag::DEFAULT);

        __TWExport Plane *CreateInifinitePlane(glm::vec3 normal, float distance, float32 width, int32 subdivision = 32, MeshDataFlag flag = MeshDataFlag::DEFAULT);

        __TWExport Sprite *CreateSprite(Texture2D::Ptr tex);
        __TWExport Sprite *CreateSprite1D(Texture1D::Ptr tex, glm::vec2 size);

#ifdef __cplusplus
    }
#endif
} // namespace TwinkleGraphics

#endif