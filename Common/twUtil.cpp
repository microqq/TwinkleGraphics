
#include "twUtil.h"
#include "twMaterialInstance.h"

namespace TwinkleGraphics
{
extern "C" __TWExport
Line::Ptr CreateLine(glm::vec3* points, int32 num)
{
    Line::Ptr line = std::make_shared<Line>(points, num);

    return line;
}

extern "C" __TWExport
Quad::Ptr CreateQuad(glm::vec2 size, MeshDataFlag flag)
{
    Quad::Ptr quad = std::make_shared<Quad>(size, flag);
    quad->GenerateMesh();

    return quad;
}

Cube::Ptr CreateCube(float32 size, MeshDataFlag flag)
{
    Cube::Ptr cube = std::make_shared<Cube>(size, flag);
    cube->GenerateMesh();

    return cube;
}

UVSphere::Ptr CreateUVSphere(float32 radius, int32 subdivision, MeshDataFlag flag)
{
    UVSphere::Ptr uvsphere = std::make_shared<UVSphere>(radius, subdivision, flag);
    uvsphere->GenerateMesh();

    return uvsphere;
}

NormalizedCubeSphere::Ptr CreateNorCubeSphere(float32 radius, int32 subdivision, MeshDataFlag flag)
{
    NormalizedCubeSphere::Ptr norcube_sphere = std::make_shared<NormalizedCubeSphere>(radius, subdivision, flag);
    norcube_sphere->GenerateMesh();

    return norcube_sphere;
}

IcosahedronSphere::Ptr CreateIcosahedronSphere(float32 radius, int32 subdivision, MeshDataFlag flag)
{
    IcosahedronSphere::Ptr icosphere = std::make_shared<IcosahedronSphere>(radius, subdivision, flag);
    icosphere->GenerateMesh();

    return icosphere;
}


BezierCurve::Ptr CreateBezierCurve(glm::vec4* points, int32 num, int32 degree, int32 segments)
{
    BezierCurve::Ptr bezier_curve = std::make_shared<BezierCurve>(points, degree, segments);
    bezier_curve->GenerateCurve();

    return bezier_curve;
}

BSplineCurve::Ptr CreateBSplineCurve(glm::vec4* points, int32 num, Knot* knots, int32 knots_num, int32 degree, int32 segments)
{
    BSplineCurve::Ptr bspline_curve = std::make_shared<BSplineCurve>(num, degree);
    bspline_curve->SetControlPoints(points, num);
    bspline_curve->SetKnots(knots, knots_num);

    bspline_curve->GenerateCurve();

    return bspline_curve;
}

NURBSSurface::Ptr CreateNURBSSurface(glm::vec4* points, Knot* uknots, Knot* vknots, int32 unum, int32 vnum, int32 udegree, int32 vdegree
        , int32 subdivide, bool rational, MeshDataFlag flag)
{
    NURBSSurface::Ptr nurbs_surface = std::make_shared<NURBSSurface>(unum, udegree, vnum, vdegree
        , subdivide, rational, flag);
    nurbs_surface->SetControlPoints(points, unum * vnum);

    nurbs_surface->SetUKnots(uknots, unum + udegree + 1);
    nurbs_surface->SetVKnots(vknots, vnum + vdegree + 1);

    nurbs_surface->GenerateMesh();

    return nurbs_surface;
}

Plane::Ptr CreatePlane(glm::vec3 normal, float32 width, int32 subdivision
    , MeshDataFlag flag)
{
    Plane::Ptr plane = std::make_shared<Plane>(normal, width, subdivision, flag);
    plane->GenerateMesh();

    MeshRenderer::Ptr plane_renderer = std::make_shared<MeshRenderer>();
    BasicGeomMaterial::Ptr plane_mat = std::make_shared<BasicGeomMaterial>();

    plane_renderer->SetSharedMaterial(plane_mat);
    plane_renderer->SetMesh(plane->GetMesh());
    plane->SetMeshRenderer(plane_renderer);

    return plane;
}

Sprite::Ptr CreateSprite(Texture2D::Ptr tex)
{
    Sprite::Ptr sprite = std::make_shared<Sprite>(tex);
    return sprite;
}

Sprite::Ptr CreateSprite1D(Texture1D::Ptr tex, glm::vec2 size)
{
    Sprite::Ptr sprite = std::make_shared<Sprite>(tex, size);
    return sprite;
}



} // namespace TwinkleGraphics
