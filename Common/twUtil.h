

#ifndef TW_UTIL_H
#define TW_UTIL_H

#include "twCommon.h"
#include "twGeometry.h"
#include "twSprite.h"


namespace TwinkleGraphics
{
extern "C" __TWExport Line* CreateLine(glm::vec3* points, int32 num);
extern "C" __TWExport Quad* CreateQuad(glm::vec2 size
    , MeshDataFlag flag = MeshDataFlag::DEFAULT);
extern "C" __TWExport Cube* CreateCube(float32 size
    , MeshDataFlag flag = MeshDataFlag::DEFAULT);
extern "C" __TWExport UVSphere* CreateUVSphere(float32 radius = 1.0f, int32 subdivision = 20
    , MeshDataFlag flag = MeshDataFlag::DEFAULT);
extern "C" __TWExport NormalizedCubeSphere* CreateNorCubeSphere(float32 radius = 1.0f, int32 subdivision = 20
    , MeshDataFlag flag = MeshDataFlag::DEFAULT);
extern "C" __TWExport IcosahedronSphere* CreateIcosahedronSphere(float32 radius = 1.0f
    , int32 subdivision = 20, MeshDataFlag flag = MeshDataFlag::DEFAULT);
extern "C" __TWExport BezierCurve* CreateBezierCurve(glm::vec4* points, int32 num, int32 degree
    , int32 segments = 128);
extern "C" __TWExport BSplineCurve* CreateBSplineCurve(glm::vec4* points, int32 num, Knot* knots, int32 knots_num
    , int32 degree, int32 segments = 128);
extern "C" __TWExport NURBSSurface* CreateNURBSSurface(glm::vec4* points, Knot* uknots, Knot* vknots, int32 unum, int32 vnum, int32 udegree, int32 vdegree
    , int32 subdivide = 16, bool rational = false, MeshDataFlag flag = MeshDataFlag(13));

extern "C" __TWExport Plane* CreatePlane(glm::vec3 normal, float32 width, int32 subdivision = 32
    , MeshDataFlag flag = MeshDataFlag::DEFAULT);

extern "C" __TWExport Plane* CreateInifinitePlane(glm::vec3 normal, float distance, float32 width, int32 subdivision = 32
    , MeshDataFlag flag = MeshDataFlag::DEFAULT);

extern "C" __TWExport Sprite* CreateSprite(Texture2D::Ptr tex);
extern "C" __TWExport Sprite* CreateSprite1D(Texture1D::Ptr tex, glm::vec2 size);

} // namespace TwinkleGraphics

#endif