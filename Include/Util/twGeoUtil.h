

#ifndef TW_GEOUTIL_H
#define TW_GEOUTIL_H

#include "twCommon.h"
#include "twGeometry.h"
#include "twSprite.h"

namespace TwinkleGraphics {
#ifdef __cplusplus
extern "C" {
#endif

#pragma region Geometry
__TWAPI Triangle *CreateTriangle(glm::vec3 p0, glm::vec3 p1,
                                       glm::vec3 p2);
__TWAPI Line *CreateLine(glm::vec3 *points, int32 num);
__TWAPI Quad *CreateQuad(glm::vec2 size,
                               MeshDataFlag flag = MeshDataFlag::DEFAULT);
__TWAPI Cube *CreateCube(float32 size,
                               MeshDataFlag flag = MeshDataFlag::DEFAULT);
__TWAPI UVSphere *
CreateUVSphere(float32 radius = 1.0f, int32 subdivision = 20,
               MeshDataFlag flag = MeshDataFlag::DEFAULT);
__TWAPI NormalizedCubeSphere *
CreateNorCubeSphere(float32 radius = 1.0f, int32 subdivision = 20,
                    MeshDataFlag flag = MeshDataFlag::DEFAULT);
__TWAPI IcosahedronSphere *
CreateIcosahedronSphere(float32 radius = 1.0f, int32 subdivision = 20,
                        MeshDataFlag flag = MeshDataFlag::DEFAULT);
__TWAPI BezierCurve *CreateBezierCurve(glm::vec4 *points, int32 num,
                                             int32 degree,
                                             int32 segments = 128);
__TWAPI BSplineCurve *CreateBSplineCurve(glm::vec4 *points, int32 num,
                                               Knot *knots, int32 knots_num,
                                               int32 degree,
                                               int32 segments = 128);
__TWAPI NURBSSurface *
CreateNURBSSurface(glm::vec4 *points, Knot *uknots, Knot *vknots, int32 unum,
                   int32 vnum, int32 udegree, int32 vdegree,
                   int32 subdivide = 16, bool rational = false,
                   MeshDataFlag flag = MeshDataFlag(13));

__TWAPI Plane *CreatePlane(glm::vec3 normal, float32 width,
                                 int32 subdivision = 32,
                                 MeshDataFlag flag = MeshDataFlag::DEFAULT);

__TWAPI Plane *
CreateInifinitePlane(glm::vec3 normal, float distance, float32 width,
                     int32 subdivision = 32,
                     MeshDataFlag flag = MeshDataFlag::DEFAULT);

__TWAPI Sprite *CreateSprite(Texture2DPtr tex);
__TWAPI Sprite *CreateSprite1D(Texture1DPtr tex, glm::vec2 size);
#pragma endregion Geometry

#ifdef __cplusplus
}
#endif

} // namespace TwinkleGraphics

#endif