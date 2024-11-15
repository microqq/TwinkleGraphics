
#include <iostream>

#include "imgui.h"

#include "twFirstTriangle.h"

namespace TwinkleGraphics {
FirstTriangle::FirstTriangle(std::string &name)
    : GLViewPlugin(name), _view(nullptr) {}

FirstTriangle::~FirstTriangle() { SAFE_DEL(_view); }

void FirstTriangle::Install() {
  Plugin::Install();

  // Initilize view
  _view = new TriangleView();
  _views[_viewsCount++] = _view;
}

void FirstTriangle::UnInstall() {
  _view->Destroy();
  SAFE_DEL(_view);

  Plugin::UnInstall();
}

void TriangleView::Initialized() {
  if (_initialized)
    return;

  Viewport viewport(Rect(0, 0, _rect.z, _rect.w), 17664U,
                    RGBA(0.0f, 0.f, 0.f, 1.f));
  CameraPtr camera = std::make_shared<Camera>(viewport, 45.0f, 0.1f, 1000.0f);
  this->SetViewCamera(camera);

  // create triangle vertices & elements(indices)
  float32 vertices[3][3] = {
      {-1.0f, -1.0f, 0.0f}, {1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}};
  uint32 elements[3] = {0, 1, 2};

  // create vertex array object
  _vaos = new uint32[1];
  glGenVertexArrays(1, _vaos);
  glBindVertexArray(_vaos[0]);

  // create vertex buffer object
  _vbos = new uint32[1];
  glGenBuffers(1, _vbos);
  _ebos = new uint32[1];
  glGenBuffers(1, _ebos);

  // bind element array buffer, bind buffer data
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebos[0]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12, elements, GL_STATIC_DRAW);

  // bind vertex array buffer, bind buffer data
  glBindBuffer(GL_ARRAY_BUFFER, _vbos[0]);
  glBufferData(GL_ARRAY_BUFFER, 36, vertices, GL_STATIC_DRAW);

  // vertex attribute layout setting
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  // create shader
  ShaderOption options[] = {
      ShaderOption::OptionData{std::string("Assets/Shaders/firstTriangle.vert"),
                               ShaderType::VERTEX_SHADER},
      ShaderOption::OptionData{std::string("Assets/Shaders/firstTriangle.frag"),
                               ShaderType::FRAGMENT_SHADER}};

  ShaderManager &shaderMgr = ShaderMgrInstance();
  _program = shaderMgr.ReadShaders(options, 2);

  // camera view setting: frustum and its position, orientation
  _camera->Translate(glm::vec3(0.0f, 0.0f, 10.0f));
  _viewMat = _camera->GetViewMatrix();
  _projectionMat = _camera->GetProjectionMatrix();

  // model matrix setting
  _modelMat = glm::mat4(1.0f);
  // scale model
  _modelMat = glm::scale(_modelMat, glm::vec3(2.f, 2.f, 2.f));

  // get shader uniform location
  _modelMatLoc =
      glGetUniformLocation(_program->GetRenderResource().id, "model");
  _viewMatLoc = glGetUniformLocation(_program->GetRenderResource().id, "view");
  _projectionMatLoc =
      glGetUniformLocation(_program->GetRenderResource().id, "projection");

  View::Initialized();
}

void TriangleView::Advance(float64 delta_time) {
  View::Advance(delta_time);

  const Viewport &viewport = _camera->GetViewport();
  _projectionMat = _camera->GetProjectionMatrix();
  _projectionMat = glm::perspective(glm::radians(45.0f), viewport.AspectRatio(),
                                    0.1f, 1000.0f);
}

void TriangleView::RenderImpl() {
  // render state setting
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);

  // bind shader program
  ShaderProgramUse useProgram(_program);

  // shader uniform setting
  glUniformMatrix4fv(_modelMatLoc, 1, GL_FALSE, glm::value_ptr(_modelMat));
  glUniformMatrix4fv(_viewMatLoc, 1, GL_FALSE, glm::value_ptr(_viewMat));
  glUniformMatrix4fv(_projectionMatLoc, 1, GL_FALSE,
                     glm::value_ptr(_projectionMat));

  // draw command use vertex array object
  glBindVertexArray(_vaos[0]);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);
}

void TriangleView::OnGUI() {

}

void TriangleView::Destroy() {
  // unbind program
  glUseProgram(0);

  // unbind vao/ebo
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Delete buffers
  glDeleteVertexArrays(1, _vaos);
  glDeleteBuffers(1, _vbos);
  glDeleteBuffers(1, _ebos);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

} // namespace TwinkleGraphics
