

#ifndef TW_UTIL_H
#define TW_UTIL_H

#include "twCommon.h"
#include "twGeometry.h"
#include "twSprite.h"


namespace TwinkleGraphics
{
static Line::Ptr CreateLine(glm::vec3* points, int32 num);
static Quad::Ptr CreateQuad(glm::vec2 size
    , MeshDataFlag flag = MeshDataFlag::DEFAULT);
static Cube::Ptr CreateCube(float32 size
    , MeshDataFlag flag = MeshDataFlag::DEFAULT);
static UVSphere::Ptr CreateUVSphere(float32 radius = 1.0f, int32 subdivision = 20
    , MeshDataFlag flag = MeshDataFlag::DEFAULT);
static NormalizedCubeSphere::Ptr CreateNorCubeSphere(float32 radius = 1.0f, int32 subdivision = 20
    , MeshDataFlag flag = MeshDataFlag::DEFAULT);
static IcosahedronSphere::Ptr CreateIcosahedronSphere(float32 radius = 1.0f
    , int32 subdivision = 20, MeshDataFlag flag = MeshDataFlag::DEFAULT);
static BezierCurve::Ptr CreateBezierCurve(glm::vec4* points, int32 num, int32 degree
    , int32 segments = 128);
static BSplineCurve::Ptr CreateBSplineCurve(glm::vec4* points, int32 num, Knot* knots, int32 knots_num
    , int32 degree, int32 segments = 128);
static NURBSSurface::Ptr CreateNURBSSurface(glm::vec4* points, Knot* uknots, Knot* vknots, int32 unum, int32 vnum, int32 udegree, int32 vdegree
    , int32 subdivide = 16, bool rational = false, MeshDataFlag flag = MeshDataFlag(13));

extern "C" __TWExport Plane::Ptr CreatePlane(glm::vec3 normal, float32 width, int32 subdivision = 32
    , MeshDataFlag flag = MeshDataFlag::DEFAULT);

static Sprite::Ptr CreateSprite(Texture2D::Ptr tex);
static Sprite::Ptr CreateSprite1D(Texture1D::Ptr tex, glm::vec2 size);

} // namespace TwinkleGraphics

#endif