
#ifndef TW_BASICGEOMETRY_H
#define TW_BASICGEOMETRY_H

#include "twCommon.h"

#include "twPlugin.h"
#include "twShader.h"
#include "twMesh.h"

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
};

class BasicGeometryView : public View
{
public:
    BasicGeometryView(Viewport& viewport)
        : View(viewport)
        , _program(nullptr)
        , _infinite_plane_program(nullptr)
        , _uvsphere(nullptr)
        , _norcubesphere(nullptr)
        , _icosphere(nullptr)
        , _cube(nullptr)
        , _quad(nullptr)
        , _line(nullptr)
        , _infinite_plane(nullptr)
        , _current_mesh(nullptr)
        , _current_mesh_index(-1)
    {
    }
    virtual ~BasicGeometryView()
    {}

protected:
    virtual void Initialize() override;
    virtual void Destroy() override;
    virtual void RenderImpl() override;
    virtual void OnGUI() override;

private:
    void CreateUVSphere();
    void CreateNorCubeSphere();
    void CreateIcoSphere();
    void CreateCube();
    void CreateQuad();
    void CreateLine();
    void CreateInfinitePlane();

    void RenderUVSphere();
    void RenderNorCubeSphere();
    void RenderIcoSphere();
    void RenderCube();
    void RenderQuad();

    void CreateGeometry(SubMesh::Ptr submesh, uint32 index);

    void RenderGeometry(Mesh::Ptr mesh, int32 index, GLenum front_face = GL_CCW);
    void RenderInfinitePlane();

private:
    uint32* _vaos;
    uint32* _vbos;
    uint32* _ebos;

    glm::mat4 _model_mat;
    glm::mat4 _view_mat;
    glm::mat4 _projection_mat;    
    glm::mat4 _mvp_mat;

    glm::vec4 _plane_param;

    ShaderProgram::Ptr _program;
    ShaderProgram::Ptr _infinite_plane_program;

    Mesh::Ptr _uvsphere;
    Mesh::Ptr _norcubesphere;
    Mesh::Ptr _icosphere;
    Mesh::Ptr _cube;
    Mesh::Ptr _quad;
    Mesh::Ptr _line;
    Mesh::Ptr _infinite_plane;
    Mesh::Ptr _current_mesh;

    uint32 _model_mat_loc;
    uint32 _view_mat_loc;
    uint32 _projection_mat_loc;

    uint32 _mvp_loc;
    uint32 _planeparam_loc;
    
    GLenum _front_face;
    int32 _current_mesh_index;

    friend class BasicGeometry;
};

} // namespace TwinkleGraphics

#endif