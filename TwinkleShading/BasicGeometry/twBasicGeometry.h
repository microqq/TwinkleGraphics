
#ifndef TW_BASICGEOMETRY_H
#define TW_BASICGEOMETRY_H

#include "twCommon.h"

#include "twPlugin.h"
#include "twShader.h"
#include "twGeometry.h"
#include "twCamera.h"
#include "twOrbitControl.h"

namespace TwinkleGraphics
{
class BasicGeometryView;

class BasicGeometry : public GLPlugin
{
public:
    BasicGeometry(std::string& name);
    ~BasicGeometry();

    virtual void Install() override;
    virtual void UnInstall() override;

private:
    BasicGeometryView* _view;
    CameraControl* _camera_control;
};

class BasicGeometryView : public View
{
public:
    BasicGeometryView()
        : View()
        , _line_points(nullptr)
        , _program(nullptr)
        , _line_program(nullptr)
        , _uvsphere(nullptr)
        , _norcubesphere(nullptr)
        , _icosphere(nullptr)
        , _cube(nullptr)
        , _quad(nullptr)
        , _line(nullptr)
        , _infinite_plane(nullptr)
        , _current_mesh(nullptr)
        , _quadbezierline(nullptr)
        , _cubicbezierline(nullptr)
        , _bspline(nullptr)
        , _nurbs_surface(nullptr)
        , _nurbs_control_line(nullptr)
        , _current_mesh_index(-1)
        , _sphere_radius(5.0f)
        , _uvsphere_longitude(50)
        , _uvsphere_latitude(25)
        , _sphere_subdivide(10)
        , _line_antialiasing(false)
        , _line_isdashed(false)
        , _line_type(false)
        , _display_infplane(false)
        , _polygon_linemode(true)
        , _cull_face(true)
    {
    }
    virtual ~BasicGeometryView()
    {
    }

protected:
    virtual void Initialize() override;
    virtual void Destroy() override;
    virtual void Advance(float64 delta_time) override;
    virtual void RenderImpl() override;
    virtual void OnGUI() override;

private:
    enum class LineJointType
    {
        BEVEL,
        ROUND,
        MITER
    };

    enum class LineCapType
    {
        BUTT,
        ROUND,
        SQUARE
    };

    void CreateUVSphere();
    void CreateNorCubeSphere();
    void CreateIcoSphere();
    void CreateCube();
    void CreateQuad();
    void CreateLine();
    void CreateInfinitePlane();
    void CreateQuadBezierLine();
    void CreateCubicBezierLine();
    void CreateBSpline();
    void CreateNURBS();
    void CreateBezierSuface();
    void CreateNURBSSurface();

    void RenderUVSphere();
    void RenderNorCubeSphere();
    void RenderIcoSphere();
    void RenderCube();
    void RenderQuad();

    void CreateGeometry(SubMesh::Ptr submesh, uint32 index);

    void RenderGeometry(Mesh::Ptr mesh, int32 index, GLenum front_face = GL_CCW);
    void RenderInfinitePlane();
    void RenderLine(Mesh::Ptr mesh, int32 index = 5);

    void OnParametersGUI();

private:
    glm::mat4 _model_mat;
    glm::mat4 _view_mat;
    glm::mat4 _projection_mat;    
    glm::mat4 _mvp_mat;

    glm::vec4 _line_params;
    glm::vec4 _viewport_params;
    glm::vec3 _line_color;

    uint32* _vaos;
    uint32* _vbos;
    uint32* _ebos;

    glm::vec3* _line_points;

    ShaderProgram::Ptr _program;
    ShaderProgram::Ptr _line_program;

    Mesh::Ptr _uvsphere;
    Mesh::Ptr _norcubesphere;
    Mesh::Ptr _icosphere;
    Mesh::Ptr _cube;
    Mesh::Ptr _quad;
    Mesh::Ptr _line;
    Mesh::Ptr _current_mesh;
    Mesh::Ptr _quadbezierline;
    Mesh::Ptr _cubicbezierline;
    BSplineCurve::Ptr _bspline;
    NURBSSurface::Ptr _nurbs_surface;
    Mesh::Ptr _nurbs_control_line;
    Plane::Ptr _infinite_plane;

    uint32 _mvp_mat_loc;

    uint32 _line_mvp_loc;
    uint32 _line_parameters_loc; //x:thickness, y:feather, z:miterlimit, w:aspect
    uint32 _line_color_loc;
    uint32 _viewport_loc;
    
    GLenum _front_face;
    int32 _current_mesh_index;

    float32 _sphere_radius;
    int32 _uvsphere_longitude;
    int32 _uvsphere_latitude;
    int32 _sphere_subdivide;

    bool _line_antialiasing;
    bool _line_isdashed;
    bool _line_type;
    bool _display_infplane;
    bool _polygon_linemode;
    bool _cull_face;

    friend class BasicGeometry;
};

} // namespace TwinkleGraphics

#endif