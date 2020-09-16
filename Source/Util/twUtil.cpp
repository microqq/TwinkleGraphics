
#include "twUtil.h"
#include "twMaterialInstance.h"

namespace TwinkleGraphics
{

#pragma region CreateSceneNode
    Triangle *CreateTriangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2)
    {
        Triangle *triangle = new Triangle(p0, p1, p2);
        // triangle->GenerateMeshInternal();

        MeshRenderer::Ptr renderer = std::make_shared<MeshRenderer>();
        BasicGeomMaterial::Ptr mat = std::make_shared<BasicGeomMaterial>();

        renderer->SetSharedMaterial(mat);
        renderer->SetMesh(triangle->GetMesh());
        triangle->SetMeshRenderer(renderer);

        return triangle;
    }

    Line *CreateLine(glm::vec3 *points, int32 num)
    {
        Line *line = new Line(points, num);

        return line;
    }

    Quad *CreateQuad(glm::vec2 size, MeshDataFlag flag)
    {
        Quad *quad = new Quad(size, flag);
        // quad->GenerateMeshInternal();

        return quad;
    }

    Cube *CreateCube(float32 size, MeshDataFlag flag)
    {
        Cube *cube = new Cube(size, flag);
        // cube->GenerateMeshInternal();

        MeshRenderer::Ptr renderer = std::make_shared<MeshRenderer>();
        CubeMaterial::Ptr mat = std::make_shared<CubeMaterial>();
        mat->SetSimpleUniformValue<float32, 1>("size", size);

        renderer->SetSharedMaterial(mat);
        renderer->SetMesh(cube->GetMesh());

        cube->SetMeshRenderer(renderer);

        return cube;
    }

    UVSphere *CreateUVSphere(float32 radius, int32 subdivision, MeshDataFlag flag)
    {
        UVSphere *uvsphere = new UVSphere(radius, subdivision, flag);
        // uvsphere->GenerateMeshInternal();

        MeshRenderer::Ptr renderer = std::make_shared<MeshRenderer>();
        SphereMaterial::Ptr mat = std::make_shared<SphereMaterial>();

        renderer->SetSharedMaterial(mat);
        renderer->SetMesh(uvsphere->GetMesh());

        uvsphere->SetMeshRenderer(renderer);

        return uvsphere;
    }

    NormalizedCubeSphere *CreateNorCubeSphere(float32 radius, int32 subdivision, MeshDataFlag flag)
    {
        NormalizedCubeSphere *norcube_sphere = new NormalizedCubeSphere(radius, subdivision, flag);
        // norcube_sphere->GenerateMeshInternal();

        MeshRenderer::Ptr renderer = std::make_shared<MeshRenderer>();
        SphereMaterial::Ptr mat = std::make_shared<SphereMaterial>();

        renderer->SetSharedMaterial(mat);
        renderer->SetMesh(norcube_sphere->GetMesh());

        norcube_sphere->SetMeshRenderer(renderer);

        return norcube_sphere;
    }

    IcosahedronSphere *CreateIcosahedronSphere(float32 radius, int32 subdivision, MeshDataFlag flag)
    {
        IcosahedronSphere *icosphere = new IcosahedronSphere(radius, subdivision, flag);
        // icosphere->GenerateMeshInternal();

        MeshRenderer::Ptr renderer = std::make_shared<MeshRenderer>();
        SphereMaterial::Ptr mat = std::make_shared<SphereMaterial>();

        renderer->SetSharedMaterial(mat);
        renderer->SetMesh(icosphere->GetMesh());

        icosphere->SetMeshRenderer(renderer);

        return icosphere;
    }

    BezierCurve *CreateBezierCurve(glm::vec4 *points, int32 numPoints, int32 degree, int32 segments)
    {
        BezierCurve *bezier_curve = new BezierCurve(points, numPoints, degree, segments);
        // bezier_curve->GenerateCurve();

        return bezier_curve;
    }

    BSplineCurve *CreateBSplineCurve(glm::vec4 *points, int32 num, Knot *knots, int32 knots_num, int32 degree, int32 segments)
    {
        BSplineCurve *bspline_curve = new BSplineCurve(num, degree, knots_num, knots, points, segments);
        // bspline_curve->SetControlPoints(points, num);
        // bspline_curve->SetKnots(knots, knots_num);
        // bspline_curve->GenerateMeshInternal();

        return bspline_curve;
    }

    NURBSSurface *CreateNURBSSurface(glm::vec4 *points, Knot *uknots, Knot *vknots, int32 unum, int32 vnum, int32 udegree, int32 vdegree, int32 subdivide, bool rational, MeshDataFlag flag)
    {
        NURBSSurface *nurbs_surface = new NURBSSurface(unum, udegree, vnum, vdegree, points, uknots, vknots, subdivide, rational, flag);
        // nurbs_surface->SetControlPoints(points, unum * vnum);

        // nurbs_surface->SetUKnots(uknots, unum + udegree + 1);
        // nurbs_surface->SetVKnots(vknots, vnum + vdegree + 1);

        // nurbs_surface->GenerateMeshInternal();

        return nurbs_surface;
    }

    Plane *CreatePlane(glm::vec3 normal, float32 width, int32 subdivision, MeshDataFlag flag)
    {
        Plane *plane = new Plane(normal, width, subdivision, flag);
        // plane->GenerateMeshInternal();

        MeshRenderer::Ptr plane_renderer = std::make_shared<MeshRenderer>();
        BasicGeomMaterial::Ptr plane_mat = std::make_shared<BasicGeomMaterial>();

        plane_renderer->SetSharedMaterial(plane_mat);
        plane_renderer->SetMesh(plane->GetMesh());
        plane->SetMeshRenderer(plane_renderer);

        return plane;
    }

    Plane *CreateInifinitePlane(glm::vec3 normal, float distance, float32 width, int32 subdivision, MeshDataFlag flag)
    {
        Plane *plane = new Plane(glm::vec3(0.0f, 0.0f, 1.0f), width, subdivision, flag);
        // plane->GenerateMeshInternal();

        MeshRenderer::Ptr plane_renderer = std::make_shared<MeshRenderer>();
        InfinitePlaneMaterial::Ptr plane_mat = std::make_shared<InfinitePlaneMaterial>();

        glm::vec4 plane_params(normal, distance);
        plane_mat->SetVecUniformValue<float32, 4>("planeParam", plane_params);

        glm::mat3 rotate_mat = glm::identity<glm::mat3>();
        float32 costheta = glm::dot(normal, glm::vec3(0.0f, 1.0f, 0.0f));
        if (costheta != 1.0f)
        {
            float32 theta = ::acosf(costheta);
            glm::vec3 axis = glm::normalize(glm::cross(normal, glm::vec3(0.0f, 1.0f, 0.0f)));
            rotate_mat = glm::mat3_cast(glm::quat(::sinf(theta * 0.5f), ::cosf(theta * 0.5f) * axis));
        }
        plane_mat->SetMatrixUniformValue<float32, 3, 3>("rotateMat", rotate_mat);

        plane_renderer->SetSharedMaterial(plane_mat);
        plane_renderer->SetMesh(plane->GetMesh());
        plane->SetMeshRenderer(plane_renderer);

        return plane;
    }

    Sprite *CreateSprite(Texture2D::Ptr tex)
    {
        Sprite *sprite = new Sprite(tex);
        return sprite;
    }

    Sprite *CreateSprite1D(Texture1D::Ptr tex, glm::vec2 size)
    {
        Sprite *sprite = new Sprite(tex, size);
        return sprite;
    }

#pragma endregion CreateSceneNode

#pragma region HashFunction
    unsigned int DJBHash(const char *str, unsigned int length)
    {
        unsigned int hash = 5381;
        unsigned int i = 0;

        for (i = 0; i < length; ++str, ++i)
        {
            hash = ((hash << 5) + hash) + (*str);
        }

        return hash;
    }
#pragma endregion HashFunction

} // namespace TwinkleGraphics
