
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
    _uvsphere = Mesh::CreateSphereMeshStandard(5.0f, 20, 10);
    SubMesh::Ptr submesh = _uvsphere->GetSubMesh(0);

    //create vertex buffer object
    _vbos = new uint32[1];
    glGenBuffers(1, _vbos);
    _ebos = new uint32[1];
    glGenBuffers(1, _ebos);

    //bind element array buffer, bind buffer data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebos[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, submesh->GetIndiceNum() * 4, submesh->GetIndice(), GL_STATIC_DRAW);

    //create vertex array object
    _vaos = new uint32[1];
    glGenVertexArrays(1, _vaos);
    glBindVertexArray(_vaos[0]);

    //bind vertex array buffer, bind buffer data
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[0]);

    glBufferData(GL_ARRAY_BUFFER, submesh->GetVerticeNum() * 12, submesh->GetVerticePos(), GL_STATIC_DRAW);
    //vertex attribute layout setting
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    //create shader
    ShaderReadInfo shaders_info[] = {
        {std::string("Assets/Shaders/basic_geometry.vert"), ShaderType::VERTEX_SHADER},
        {std::string("Assets/Shaders/basic_geometry.frag"), ShaderType::FRAGMENT_SHADER}};

    ShaderManagerInst shaderMgr;
    _program = shaderMgr->ReadShaders(shaders_info, 2);

    //camera view setting: frustum and its position, orientation
    glm::vec3 eye(0.0f, 0.0f, 10.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    glm::vec3 center(0.0f, 0.0f, 0.0f);
    _view_mat = glm::lookAtRH(eye, center, up);

    _projection_mat = glm::perspective(glm::radians(45.0f), _viewport.AspectRatio(), 0.1f, 100.0f);

    //model matrix setting
    _model_mat = glm::mat4(1.0f);
    //scale model
    _model_mat = glm::scale(_model_mat, glm::vec3(0.5f, 0.5f, 0.5f));

    //get shader uniform location
    _model_mat_loc = glGetUniformLocation(_program->GetRes().id, "model");
    _view_mat_loc = glGetUniformLocation(_program->GetRes().id, "view");
    _projection_mat_loc = glGetUniformLocation(_program->GetRes().id, "projection");
}

void BasicGeometryView::RenderImpl()
{
    //render state setting
    // glDisable(GL_CULL_FACE);
    // glDisable(GL_DEPTH_TEST);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //bind shader program
    ShaderProgramUse useProgram(_program);

    //shader uniform setting
    glUniformMatrix4fv(_model_mat_loc, 1, GL_FALSE, glm::value_ptr(_model_mat));
    glUniformMatrix4fv(_view_mat_loc, 1, GL_FALSE, glm::value_ptr(_view_mat));
    glUniformMatrix4fv(_projection_mat_loc, 1, GL_FALSE, glm::value_ptr(_projection_mat));

    SubMesh::Ptr submesh = _uvsphere->GetSubMesh(0);

    //draw command use vertex array object
    glBindVertexArray(_vaos[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebos[0]);
    glDrawElements(GL_TRIANGLES, submesh->GetIndiceNum(), GL_UNSIGNED_INT, NULL);
}

void BasicGeometryView::OnGUI()
{
}

void BasicGeometryView::Destroy()
{
    //unbind program
    glUseProgram(0);

    //unbind vao/ebo
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //Delete buffers
    glDeleteVertexArrays(1, _vaos);
    glDeleteBuffers(1, _vbos);
    glDeleteBuffers(1, _ebos);
}

} // namespace TwinkleGraphics
