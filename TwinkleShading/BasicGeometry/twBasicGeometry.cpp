
#include <iostream>

#include "imgui.h"

#include "twBasicGeometry.h"

namespace TwinkleGraphics
{
BasicGeometry::BasicGeometry(std::string& name)
    : GLPlugin(name)
    , _view(nullptr)
{
}

BasicGeometry::~BasicGeometry()
{
}

void BasicGeometry::Install()
{
    Plugin::Install();

    // Initilize view
    Viewport viewport(Rect(0, 0, 1024, 768), 17664U, RGBA(0.0f, 0.f, 0.f, 1.f));
    _view = new BasicGeometryView(viewport);
    _view->Initialize();

    _views[_views_count++] = _view;
}

void BasicGeometry::UnInstall()
{
    _view->Destroy();
    SAFE_DEL(_view);

    Plugin::UnInstall();
}



void BasicGeometryView::Initialize()
{
    //create vertex buffer object
    _vbos = new uint32[7];
    glGenBuffers(7, _vbos);
    _ebos = new uint32[7];
    glGenBuffers(7, _ebos);

    //create vertex array object
    _vaos = new uint32[7];
    glGenVertexArrays(7, _vaos);

    //create sphere
    CreateUVSphere();
    CreateNorCubeSphere();
    CreateIcoSphere();
    CreateQuad();
    CreateCube();
    CreateInfinitePlane();

    //camera view setting: frustum and its position, orientation
    // glm::vec3 eye(0.0f, 10.0f, 0.0f);
    glm::vec3 eye(0.0f, 5.0f, 15.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    glm::vec3 center(0.0f, 0.0f, 0.0f);
    _view_mat = glm::lookAtRH(eye, center, up);
    _projection_mat = glm::perspective(glm::radians(45.0f), _viewport.AspectRatio(), 0.1f, 1000.0f);
    _model_mat = glm::mat4(1.0f);

    _mvp_mat = _projection_mat * _view_mat;

    //model transformation
    // _model_mat = glm::rotate(_model_mat, glm::radians<float32>(-90), glm::vec3(1.0f, 0.0f, 0.0f));
    _model_mat = glm::scale(_model_mat, glm::vec3(0.5f, 0.5f, 0.5f));

    ShaderManagerInst shaderMgr;

    //create basic-geometry shader
    ShaderReadInfo shaders_info[] = {
        {std::string("Assets/Shaders/basic_geometry.vert"), ShaderType::VERTEX_SHADER},
        {std::string("Assets/Shaders/basic_geometry.frag"), ShaderType::FRAGMENT_SHADER}};

    _program = shaderMgr->ReadShaders(shaders_info, 2);
    {
        ShaderProgramUse use_program(_program);
        //get shader uniform location
        _model_mat_loc = glGetUniformLocation(_program->GetRes().id, "model");
        _view_mat_loc = glGetUniformLocation(_program->GetRes().id, "view");
        _projection_mat_loc = glGetUniformLocation(_program->GetRes().id, "projection");
    }


    _plane_param = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    //create basic-geometry shader
    ShaderReadInfo infplane_shaders_info[] = {
        {std::string("Assets/Shaders/infinite_plane.vert"), ShaderType::VERTEX_SHADER},
        {std::string("Assets/Shaders/infinite_plane.frag"), ShaderType::FRAGMENT_SHADER}};

    _infinite_plane_program = shaderMgr->ReadShaders(infplane_shaders_info, 2);
    {
        ShaderProgramUse use_program(_infinite_plane_program);
        _mvp_loc = glGetUniformLocation(_infinite_plane_program->GetRes().id, "mvp");
        _planeparam_loc = glGetUniformLocation(_infinite_plane_program->GetRes().id, "plane_param");
    }
}

void BasicGeometryView::RenderImpl()
{
    // RenderUVSphere();

    // RenderNorCubeSphere();

    // RenderIcoSphere();

    // RenderQuad();

    // RenderCube();

    if(_current_mesh != nullptr)
    {
        if( _current_mesh_index != -1 &&
            _current_mesh_index != 6)
        {
            RenderGeometry(_current_mesh, _current_mesh_index, _front_face);
        }
        else if(_current_mesh_index == 6)
        {
            RenderInfinitePlane();
        }
    }
}

void BasicGeometryView::OnGUI()
{
    ImGui::Begin(u8"创建几何形体");
    {
        if(ImGui::RadioButton(u8"球体(UV Sphere)", &_current_mesh_index, 0))
        {
            if(_uvsphere == nullptr)
            {
                CreateUVSphere();
            }
            _front_face = GL_CW;
            _current_mesh = _uvsphere;
        }
        if(ImGui::RadioButton(u8"球体(Nor Cube Sphere)", &_current_mesh_index, 1))
        {
            if(_norcubesphere == nullptr)
            {
                CreateNorCubeSphere();
            }
            _front_face = GL_CCW;
            _current_mesh = _norcubesphere;
        }
        if(ImGui::RadioButton(u8"球体(Ico Sphere)", &_current_mesh_index, 2))
        {
            if(_icosphere == nullptr)
            {
                CreateIcoSphere();
            }
            _front_face = GL_CCW;
            _current_mesh = _icosphere;
        }
        if(ImGui::RadioButton(u8"四边形", &_current_mesh_index, 4))
        {
            if(_quad == nullptr)
            {
                CreateQuad();
            }
            _front_face = GL_CCW;
            _current_mesh = _quad;
        }
        if(ImGui::RadioButton(u8"立方体", &_current_mesh_index, 3))
        {
            if(_cube == nullptr)
            {
                CreateCube();
            }
            _front_face = GL_CCW;
            _current_mesh = _cube;
        }
        if(ImGui::RadioButton(u8"线", &_current_mesh_index, 5))
        {
            if(_line == nullptr)
            {
                CreateLine();
            }
            _front_face = GL_CCW;
            _current_mesh = _line;
        }
        if(ImGui::RadioButton(u8"无限平面", &_current_mesh_index, 6))
        {
            if(_infinite_plane == nullptr)
            {
                CreateInfinitePlane();
            }
            _front_face = GL_CCW;
            _current_mesh = _infinite_plane;
        }
    }
    ImGui::End();
}

void BasicGeometryView::Destroy()
{
    //unbind program
    glUseProgram(0);

    //unbind vao/ebo
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //Delete buffers
    glDeleteVertexArrays(7, _vaos);
    glDeleteBuffers(7, _vbos);
    glDeleteBuffers(7, _ebos);
}

void BasicGeometryView::CreateGeometry(SubMesh::Ptr submesh, uint32 index)
{
    //bind element array buffer, bind buffer data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebos[index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, submesh->GetIndiceNum() * 4, submesh->GetIndice(), GL_STATIC_DRAW);

    //bind vertex array object
    glBindVertexArray(_vaos[index]);

    //bind vertex array buffer, bind buffer data
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[index]);

    glBufferData(GL_ARRAY_BUFFER, submesh->GetVerticeNum() * 12, submesh->GetVerticePos(), GL_STATIC_DRAW);
    //vertex attribute layout setting
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
}

void BasicGeometryView::RenderGeometry(Mesh::Ptr mesh, int32 index, GLenum front_face)
{
    //render state setting
    // glDisable(GL_DEPTH_TEST);
    // glDisable(GL_CULL_FACE);

    glFrontFace(front_face);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //bind shader program
    ShaderProgramUse use_program(_program);

    //shader uniform setting
    glUniformMatrix4fv(_model_mat_loc, 1, GL_FALSE, glm::value_ptr(_model_mat));
    glUniformMatrix4fv(_view_mat_loc, 1, GL_FALSE, glm::value_ptr(_view_mat));
    glUniformMatrix4fv(_projection_mat_loc, 1, GL_FALSE, glm::value_ptr(_projection_mat));

    SubMesh::Ptr submesh = mesh->GetSubMesh(0);

    //draw command use vertex array object
    glBindVertexArray(_vaos[index]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebos[index]);
    glDrawElements(GL_TRIANGLES, submesh->GetIndiceNum(), GL_UNSIGNED_INT, NULL);
    // glDrawElements(GL_POINTS, submesh->GetIndiceNum(), GL_UNSIGNED_INT, NULL);


    glFrontFace(GL_CCW);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void BasicGeometryView::RenderInfinitePlane()
{
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    ShaderProgramUse use_program(_infinite_plane_program);
    {
        glUniformMatrix4fv(_mvp_loc, 1, GL_FALSE, glm::value_ptr(_mvp_mat));
        glUniform4fv(_planeparam_loc, 1, glm::value_ptr(_plane_param));

        SubMesh::Ptr submesh = _infinite_plane->GetSubMesh(0);

        //draw command use vertex array object
        glBindVertexArray(_vaos[6]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebos[6]);
        glDrawElements(GL_TRIANGLES, submesh->GetIndiceNum(), GL_UNSIGNED_INT, NULL);
    }
}


/**
 * @brief Render UV Sphere
 * 
 */
void BasicGeometryView::RenderUVSphere()
{
    RenderGeometry(_uvsphere, 0, GL_CW);
}

void BasicGeometryView::CreateUVSphere() 
{
    _uvsphere = Mesh::CreateSphereMeshStandard(5.0f, 50, 25);
    SubMesh::Ptr submesh = _uvsphere->GetSubMesh(0);

    CreateGeometry(submesh, 0);
}


/**
 * @brief Render Normalized Cube Subdivide Sphere
 * 
 */
void BasicGeometryView::RenderNorCubeSphere()
{
    RenderGeometry(_norcubesphere, 1, GL_CCW);
}

void BasicGeometryView::CreateNorCubeSphere() 
{
    _norcubesphere = Mesh::CreateSphereMeshNormalizedCube(5.0f, 20);
    SubMesh::Ptr submesh = _norcubesphere->GetSubMesh(0);

    CreateGeometry(submesh, 1);
}

void BasicGeometryView::RenderIcoSphere()
{
    RenderGeometry(_icosphere, 2, GL_CCW);
}

void BasicGeometryView::CreateIcoSphere() 
{
    _icosphere = Mesh::CreateSphereMeshIcosahedron(5.0f, 10);
    SubMesh::Ptr submesh = _icosphere->GetSubMesh(0);

    CreateGeometry(submesh, 2);
}

void BasicGeometryView::RenderCube()
{
    RenderGeometry(_cube, 3, GL_CCW);
}

void BasicGeometryView::CreateCube() 
{
    _cube = Mesh::CreateCubeMesh(5.0f);
    SubMesh::Ptr submesh = _cube->GetSubMesh(0);
    CreateGeometry(submesh, 3);
}

void BasicGeometryView::RenderQuad()
{
    RenderGeometry(_quad, 4, GL_CCW);
}

void BasicGeometryView::CreateQuad() 
{
    _quad = Mesh::CreateQuadMesh(5.0f, 5.0f);
    SubMesh::Ptr submesh = _quad->GetSubMesh(0);
    CreateGeometry(submesh, 4);
}

void BasicGeometryView::CreateLine()
{
}

void BasicGeometryView::CreateInfinitePlane()
{
    _infinite_plane = Mesh::CreateQuadMesh(2.0f, 2.0f);
    SubMesh::Ptr submesh = _infinite_plane->GetSubMesh(0);
    CreateGeometry(submesh, 6);
}

} // namespace TwinkleGraphics
