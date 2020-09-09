

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

#pragma region CreateSceneNode
        __TWUTILExport Triangle *CreateTriangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);
        __TWUTILExport Line *CreateLine(glm::vec3 *points, int32 num);
        __TWUTILExport Quad *CreateQuad(glm::vec2 size, MeshDataFlag flag = MeshDataFlag::DEFAULT);
        __TWUTILExport Cube *CreateCube(float32 size, MeshDataFlag flag = MeshDataFlag::DEFAULT);
        __TWUTILExport UVSphere *CreateUVSphere(float32 radius = 1.0f, int32 subdivision = 20, MeshDataFlag flag = MeshDataFlag::DEFAULT);
        __TWUTILExport NormalizedCubeSphere *CreateNorCubeSphere(float32 radius = 1.0f, int32 subdivision = 20, MeshDataFlag flag = MeshDataFlag::DEFAULT);
        __TWUTILExport IcosahedronSphere *CreateIcosahedronSphere(float32 radius = 1.0f, int32 subdivision = 20, MeshDataFlag flag = MeshDataFlag::DEFAULT);
        __TWUTILExport BezierCurve *CreateBezierCurve(glm::vec4 *points, int32 num, int32 degree, int32 segments = 128);
        __TWUTILExport BSplineCurve *CreateBSplineCurve(glm::vec4 *points, int32 num, Knot *knots, int32 knots_num, int32 degree, int32 segments = 128);
        __TWUTILExport NURBSSurface *CreateNURBSSurface(glm::vec4 *points, Knot *uknots, Knot *vknots, int32 unum, int32 vnum, int32 udegree, int32 vdegree, int32 subdivide = 16, bool rational = false, MeshDataFlag flag = MeshDataFlag(13));

        __TWUTILExport Plane *CreatePlane(glm::vec3 normal, float32 width, int32 subdivision = 32, MeshDataFlag flag = MeshDataFlag::DEFAULT);

        __TWUTILExport Plane *CreateInifinitePlane(glm::vec3 normal, float distance, float32 width, int32 subdivision = 32, MeshDataFlag flag = MeshDataFlag::DEFAULT);

        __TWUTILExport Sprite *CreateSprite(Texture2D::Ptr tex);
        __TWUTILExport Sprite *CreateSprite1D(Texture1D::Ptr tex, glm::vec2 size);
#pragma endregion CreateSceneNode

#pragma region HashFunction // https://www.partow.net/programming/hashfunctions/#DJBHashFunction
        __TWUTILExport unsigned int DJBHash(const char *str, unsigned int length);
#pragma endregion HashFunction
#ifdef __cplusplus
        }
#endif
} // namespace TwinkleGraphics

#endif