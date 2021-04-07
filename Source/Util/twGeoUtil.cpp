
#include "twGeoUtil.h"
#include "twMaterialInstance.h"

namespace TwinkleGraphics
{

#pragma region CreateSceneNode
    Triangle *CreateTriangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2)
    {
        Triangle *triangle = new Triangle(p0, p1, p2);
        // triangle->GenerateMeshInternal();

        MeshRendererPtr renderer = std::make_shared<MeshRenderer>();
        MaterialPtr mat = std::make_shared<BasicGeomMaterial>();
        renderer->SetMaterial(mat);
        mat = renderer->GetMaterial();

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

        MeshRendererPtr renderer = std::make_shared<MeshRenderer>();
        MaterialPtr mat = std::make_shared<CubeMaterial>();
        renderer->SetMaterial(mat);
        mat = renderer->GetMaterial();
        mat->SetSimpleUniformValue<float32, 1>("size", size);

        renderer->SetMesh(cube->GetMesh());
        cube->SetMeshRenderer(renderer);

        return cube;
    }

    UVSphere *CreateUVSphere(float32 radius, int32 subdivision, MeshDataFlag flag)
    {
        UVSphere *uvsphere = new UVSphere(radius, subdivision, flag);
        // uvsphere->GenerateMeshInternal();

        MeshRendererPtr renderer = std::make_shared<MeshRenderer>();
        MaterialPtr mat = std::make_shared<SphereMaterial>();
        renderer->SetMaterial(mat);
        mat = renderer->GetMaterial();

        renderer->SetMesh(uvsphere->GetMesh());
        uvsphere->SetMeshRenderer(renderer);

        return uvsphere;
    }

    NormalizedCubeSphere *CreateNorCubeSphere(float32 radius, int32 subdivision, MeshDataFlag flag)
    {
        NormalizedCubeSphere *norcube_sphere = new NormalizedCubeSphere(radius, subdivision, flag);
        // norcube_sphere->GenerateMeshInternal();

        MeshRendererPtr renderer = std::make_shared<MeshRenderer>();
        MaterialPtr mat = std::make_shared<SphereMaterial>();
        renderer->SetMaterial(mat);
        mat = renderer->GetMaterial();

        renderer->SetMesh(norcube_sphere->GetMesh());
        norcube_sphere->SetMeshRenderer(renderer);

        return norcube_sphere;
    }

    IcosahedronSphere *CreateIcosahedronSphere(float32 radius, int32 subdivision, MeshDataFlag flag)
    {
        IcosahedronSphere *icosphere = new IcosahedronSphere(radius, subdivision, flag);
        // icosphere->GenerateMeshInternal();

        MeshRendererPtr renderer = std::make_shared<MeshRenderer>();
        MaterialPtr mat = std::make_shared<SphereMaterial>();
        renderer->SetMaterial(mat);
        mat = renderer->GetMaterial();

        renderer->SetMesh(icosphere->GetMesh());
        icosphere->SetMeshRenderer(renderer);

        return icosphere;
    }

    BezierCurve *CreateBezierCurve(glm::vec4 *points, int32 numPoints, int32 degree, int32 segments)
    {
        BezierCurve *bezierCurve = new BezierCurve(points, numPoints, degree, segments);
        // bezierCurve->GenerateCurve();

        return bezierCurve;
    }

    BSplineCurve *CreateBSplineCurve(glm::vec4 *points, int32 num, Knot *knots, int32 knots_num, int32 degree, int32 segments)
    {
        BSplineCurve *bsplineCurve = new BSplineCurve(num, degree, knots_num, knots, points, segments);
        // bsplineCurve->SetControlPoints(points, num);
        // bsplineCurve->SetKnots(knots, knots_num);
        // bsplineCurve->GenerateMeshInternal();

        return bsplineCurve;
    }

    NURBSSurface *CreateNURBSSurface(glm::vec4 *points, Knot *uknots, Knot *vknots, int32 unum, int32 vnum, int32 udegree, int32 vdegree, int32 subdivide, bool rational, MeshDataFlag flag)
    {
        NURBSSurface *nurbsSurface = new NURBSSurface(unum, udegree, vnum, vdegree, points, uknots, vknots, subdivide, rational, flag);
        // nurbsSurface->SetControlPoints(points, unum * vnum);

        // nurbsSurface->SetUKnots(uknots, unum + udegree + 1);
        // nurbsSurface->SetVKnots(vknots, vnum + vdegree + 1);

        // nurbsSurface->GenerateMeshInternal();

        return nurbsSurface;
    }

    Plane *CreatePlane(glm::vec3 normal, float32 width, int32 subdivision, MeshDataFlag flag)
    {
        Plane *plane = new Plane(normal, width, subdivision, flag);
        // plane->GenerateMeshInternal();

        MeshRendererPtr planeRenderer = std::make_shared<MeshRenderer>();
        MaterialPtr planeMat = std::make_shared<BasicGeomMaterial>();
        planeRenderer->SetMaterial(planeMat);
        planeMat = planeRenderer->GetMaterial();

        planeRenderer->SetMesh(plane->GetMesh());
        plane->SetMeshRenderer(planeRenderer);

        return plane;
    }

    Plane *CreateInifinitePlane(glm::vec3 normal, float distance, float32 width, int32 subdivision, MeshDataFlag flag)
    {
        Plane *plane = new Plane(glm::vec3(0.0f, 0.0f, 1.0f), width, subdivision, flag);
        // plane->GenerateMeshInternal();

        MeshRendererPtr planeRenderer = std::make_shared<MeshRenderer>();
        MaterialPtr planeMat = std::make_shared<InfinitePlaneMaterial>();
        planeRenderer->SetMaterial(planeMat);
        planeMat = planeRenderer->GetMaterial();

        glm::vec4 planeParams(normal, distance);
        planeMat->SetVecUniformValue<float32, 4>("planeParam", planeParams);

        glm::mat3 rotate_mat = glm::identity<glm::mat3>();
        float32 costheta = glm::dot(normal, glm::vec3(0.0f, 1.0f, 0.0f));
        if (costheta != 1.0f)
        {
            float32 theta = ::acosf(costheta);
            glm::vec3 axis = glm::normalize(glm::cross(normal, glm::vec3(0.0f, 1.0f, 0.0f)));
            rotate_mat = glm::mat3_cast(glm::quat(::sinf(theta * 0.5f), ::cosf(theta * 0.5f) * axis));
        }
        planeMat->SetMatrixUniformValue<float32, 3, 3>("rotateMat", rotate_mat);

        planeRenderer->SetMesh(plane->GetMesh());
        plane->SetMeshRenderer(planeRenderer);

        return plane;
    }

    Sprite *CreateSprite(Texture2DPtr tex)
    {
        Sprite *sprite = new Sprite(tex);
        return sprite;
    }

    Sprite *CreateSprite1D(Texture1DPtr tex, glm::vec2 size)
    {
        Sprite *sprite = new Sprite(tex, size);
        return sprite;
    }

#pragma endregion CreateSceneNode




} // namespace TwinkleGraphics
