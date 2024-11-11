
#include <iostream>

#include "imgui.h"
#include "twBasicGeometry.h"
#include "twGeoUtil.h"
#include "twImageManager.h"
#include "twImGuiContextManager.h"

namespace TwinkleGraphics {
BasicGeometry::BasicGeometry(std::string &name)
    : GLViewPlugin(name), _view(nullptr) {}

BasicGeometry::~BasicGeometry() { SAFE_DEL(_view); }

void BasicGeometry::Install() {
  Plugin::Install();

  // Initilize view
  _view = new BasicGeometryView();
  _views[_viewsCount++] = _view;
}

void BasicGeometry::UnInstall() {
  _view->Destroy();
  SAFE_DEL(_view);

  Plugin::UnInstall();
}

void BasicGeometryView::Initialized() {
  if (_initialized)
    return;

  Viewport viewport(Rect(0, 0, _rect.z, _rect.w), 17664U,
                    RGBA(0.0f, 0.f, 0.f, 1.f));
  CameraPtr camera = std::make_shared<Camera>(viewport, 45.0f, 1.0f, 5000.0f);
  this->SetViewCamera(camera);

  _cameraControl = std::make_shared<FirstPersonControl>(camera);
  (dynamic_cast<FirstPersonControl *>(_cameraControl.get()))
      ->SetMinDistance(-5000.0f);
  (dynamic_cast<FirstPersonControl *>(_cameraControl.get()))
      ->SetMaxDistance(5000.0f);
  (dynamic_cast<FirstPersonControl *>(_cameraControl.get()))
      ->SetDistance(25.0f);
  this->SetCameraControl(_cameraControl);

  // create vertex buffer object
  _vbos = new uint32[20];
  glGenBuffers(20, _vbos);
  _ebos = new uint32[20];
  glGenBuffers(20, _ebos);

  // create vertex array object
  _vaos = new uint32[20];
  glGenVertexArrays(20, _vaos);

  // create sphere
  CreateUVSphere();
  CreateNorCubeSphere();
  CreateIcoSphere();
  CreateQuad();
  CreateCube();
  CreateInfinitePlane();

  // camera view setting: frustum and its position, orientation
  _viewMat = glm::mat4(_camera->GetViewMatrix());
  _projectionMat = glm::mat4(_camera->GetProjectionMatrix());
  _mvpMat = _projectionMat * _viewMat;

  ShaderManager &shaderMgr = ShaderMgrInstance();

  // create basic-geometry shader
  ShaderOption options[] = {
      ShaderOption::OptionData{std::string("Assets/Shaders/basicGeometry.vert"),
                               ShaderType::VERTEX_SHADER},
      ShaderOption::OptionData{std::string("Assets/Shaders/basicGeometry.frag"),
                               ShaderType::FRAGMENT_SHADER}};

  _program = shaderMgr.ReadShaders(options, 2);
  {
    ShaderProgramUse use_program(_program);
    // get shader uniform location
    _mvpMatLoc = glGetUniformLocation(_program->GetRenderResource().id, "mvp");
    int32 tintcolor_loc =
        glGetUniformLocation(_program->GetRenderResource().id, "tintColor");

    glm::vec4 tintcolor(1.0f, 1.0f, 1.0f, 1.0f);
    glUniform4fv(tintcolor_loc, 1, glm::value_ptr(tintcolor));
  }

  // create infinite-plane shader
  ShaderOption infplane_shader_options[] = {
      ShaderOption::OptionData{std::string("Assets/Shaders/infinitePlane.vert"),
                               ShaderType::VERTEX_SHADER},
      ShaderOption::OptionData{std::string("Assets/Shaders/infinitePlane.frag"),
                               ShaderType::FRAGMENT_SHADER}};

  _viewportParams =
      glm::vec4((float32)(viewport.Width()), (float32)(viewport.Height()),
                viewport.AspectRatio(), 1.0f);
  _lineParams = glm::vec4(0.01f, 0.002f, 0.99f, 1.0f);
  _lineColor = glm::vec3(1.0f, 1.0f, 1.0f);
  _linePoints = new glm::vec3[5]{
      glm::vec3(-5.f, 2.5f, 0.0f), glm::vec3(0.0f, -5.0f, 0.0f),
      glm::vec3(-3.f, 2.0f, 0.0f), glm::vec3(5.f, 2.5f, 0.0f),
      glm::vec3(-5.f, -2.5f, -5.0f)};
  CreateLine();

  ShaderOption line_shader_options[] = {
      ShaderOption::OptionData{std::string("Assets/Shaders/line.vert"),
                               ShaderType::VERTEX_SHADER},
      ShaderOption::OptionData{std::string("Assets/Shaders/line.geom"),
                               ShaderType::GEOMETRY_SHADER},
      ShaderOption::OptionData{std::string("Assets/Shaders/line.frag"),
                               ShaderType::FRAGMENT_SHADER}};
  _lineProgram = shaderMgr.ReadShaders(line_shader_options, 3);
  {
    ShaderProgramUse use_program(_lineProgram);
    _lineMVPLoc =
        glGetUniformLocation(_lineProgram->GetRenderResource().id, "mvp");
    _lineParametersLoc = glGetUniformLocation(
        _lineProgram->GetRenderResource().id, "lineParams");
    _viewportLoc = glGetUniformLocation(_lineProgram->GetRenderResource().id,
                                        "viewportParams");
    _lineColorLoc =
        glGetUniformLocation(_lineProgram->GetRenderResource().id, "lineColor");
  }

  View::Initialized();
}

void BasicGeometryView::Advance(float64 delta_time) {
  View::Advance(delta_time);

  const Viewport &viewport = _camera->GetViewport();
  _viewportParams =
      glm::vec4((float32)(viewport.Width()), (float32)(viewport.Height()),
                viewport.AspectRatio(), 1.0f);
  _viewMat = _camera->GetViewMatrix();
  _projectionMat = _camera->GetProjectionMatrix();
  _mvpMat = _projectionMat * _viewMat;

  MaterialPtr material = _infinitePlane->GetMeshRenderer()->GetMaterial();
  material->SetMatrixUniformValue<float32, 4, 4>("mvp", _mvpMat);
}

void BasicGeometryView::RenderImpl() {
  // RenderUVSphere();

  // RenderNorCubeSphere();

  // RenderIcoSphere();

  // RenderQuad();

  // RenderCube();

  if (_currentMesh != nullptr) {
    if (_currentMeshIndex == 6) {
      RenderInfinitePlane();
    } else if (_currentMeshIndex == 5) {
      if (_displayInfiniteplane) {
        RenderInfinitePlane();
      }
      RenderLine(_line);
    } else if (_currentMeshIndex == 7) {
      if (_displayInfiniteplane) {
        RenderInfinitePlane();
      }
      RenderLine(_quadbezierline, 7);
    } else if (_currentMeshIndex == 8) {
      if (_displayInfiniteplane) {
        RenderInfinitePlane();
      }
      RenderLine(_cubicbezierline, 8);
    } else if (_currentMeshIndex == 9) {
      if (_displayInfiniteplane) {
        RenderInfinitePlane();
      }
      RenderLine(_bspline->GetMesh(), 9);
    } else if (_currentMeshIndex != -1) {
      // //nurbs surface control line
      // if(_current_mesh_index == 10)
      // {
      //     RenderLine(_nurbs_control_line, 19);
      // }
      if (_displayInfiniteplane) {
        RenderInfinitePlane();
      }
      RenderGeometry(_currentMesh, _currentMeshIndex, _frontFace);
    }
  }
}

void BasicGeometryView::OnGUI() {
  // ImGui::Begin(u8"相机参数");
  // {
  //     bool changed = ImGui::InputFloat3(u8"相机位置", glm::value_ptr(_eye));
  //     changed |= ImGui::InputFloat3(u8"视线中心", glm::value_ptr(_center));
  //     changed |= ImGui::InputFloat3(u8"向上向量", glm::value_ptr(_up));

  //     if(changed)
  //     {
  //         _view_mat = glm::lookAtRH(_eye, _center, _up);
  //         _mvp_mat = _projection_mat * _view_mat;
  //     }
  // }
  // ImGui::End();

  ImGuiContextManager &imguiCtxMgr = ImGuiContextMgrInstance();
  imguiCtxMgr.SetCurrentContext();
  ImGui::Begin(u8"创建几何形体");
  {
    if (ImGui::RadioButton(u8"球体(UV Sphere)", &_currentMeshIndex, 0)) {
      if (_uvsphere == nullptr) {
        CreateUVSphere();
      }
      _frontFace = GL_CW;
      _currentMesh = _uvsphere;
    }
    if (ImGui::RadioButton(u8"球体(Nor Cube Sphere)", &_currentMeshIndex, 1)) {
      if (_norcubesphere == nullptr) {
        CreateNorCubeSphere();
      }
      _frontFace = GL_CCW;
      _currentMesh = _norcubesphere;
    }
    if (ImGui::RadioButton(u8"球体(Ico Sphere)", &_currentMeshIndex, 2)) {
      if (_icosphere == nullptr) {
        CreateIcoSphere();
      }
      _frontFace = GL_CCW;
      _currentMesh = _icosphere;
    }
    if (ImGui::RadioButton(u8"四边形", &_currentMeshIndex, 4)) {
      if (_quad == nullptr) {
        CreateQuad();
      }
      _frontFace = GL_CCW;
      _currentMesh = _quad;
    }
    if (ImGui::RadioButton(u8"立方体", &_currentMeshIndex, 3)) {
      if (_cube == nullptr) {
        CreateCube();
      }
      _frontFace = GL_CCW;
      _currentMesh = _cube;
    }
    if (ImGui::RadioButton(u8"线", &_currentMeshIndex, 5)) {
      if (_line == nullptr) {
        CreateLine();
      }
      _frontFace = GL_CCW;
      _currentMesh = _line;
    }
    if (ImGui::RadioButton(u8"无限平面", &_currentMeshIndex, 6)) {
      if (_infinitePlane == nullptr) {
        CreateInfinitePlane();
      }
      _frontFace = GL_CCW;
      _currentMesh = _infinitePlane->GetMesh();
    }
    if (ImGui::RadioButton(u8"二阶贝塞尔曲线", &_currentMeshIndex, 7)) {
      if (_quadbezierline == nullptr) {
        CreateQuadBezierLine();
      }
      _frontFace = GL_CCW;
      _currentMesh = _quadbezierline;
    }
    if (ImGui::RadioButton(u8"三阶贝塞尔曲线", &_currentMeshIndex, 8)) {
      if (_cubicbezierline == nullptr) {
        CreateCubicBezierLine();
      }
      _frontFace = GL_CCW;
      _currentMesh = _cubicbezierline;
    }
    if (ImGui::RadioButton(u8"B-样条曲线", &_currentMeshIndex, 9)) {
      if (_bspline == nullptr) {
        CreateBSpline();
      }
      _frontFace = GL_CCW;
      _currentMesh = _bspline->GetMesh();
    }
    if (ImGui::RadioButton(u8"NURBS-曲面", &_currentMeshIndex, 10)) {
      if (_nurbsSurface == nullptr) {
        CreateNURBSSurface();
      }
      _frontFace = GL_CCW;
      _currentMesh = _nurbsSurface->GetMesh();
    }
  }
  ImGui::End();

  imguiCtxMgr.SetCurrentContext();
  OnParametersGUI();
}

void BasicGeometryView::OnParametersGUI() {
  if (_currentMeshIndex != -1) {
    ImGui::Begin(u8"参数设置");
    ImGui::Checkbox(u8"剔除背面", &_cullFace);
    ImGui::Checkbox(u8"线框模式", &_polygonLineMode);
    ImGui::Checkbox(u8"显示平面", &_displayInfiniteplane);

    if (_currentMeshIndex == 5 || _currentMeshIndex == 7 ||
        _currentMeshIndex == 8 || _currentMeshIndex == 9) {
      // if(_current_mesh == _line)
      {
        ImGui::InputFloat(u8"线宽", &(_lineParams.x), 0.01f);
        ImGui::Checkbox(u8"抗锯齿", &_lineAntialiasing);
        {
          if (_lineAntialiasing) {
            ImGui::InputFloat(u8"羽化", &(_lineParams.y), 0.001f);
            _lineParams.w = 1.0f;
          } else {
            _lineParams.w = 0.0f;
          }
        }
        ImGui::InputFloat(u8"连接点限制参数", &(_lineParams.z), 0.001f);
        ImGui::Checkbox(u8"虚线", &_lineIsdashed);
        {}
        ImGui::Checkbox(u8"圆角直线", &_lineType);
        {}

        ImGui::DragFloat3(u8"颜色", glm::value_ptr(_lineColor), 0.01f, 0.0f,
                          1.0f);
      }
    } else if (_currentMeshIndex == 0) {
      bool recreate = false;
      recreate = ImGui::InputFloat(u8"球半径", &_sphereRadius, 0.5f, 0.0f);
      recreate |= ImGui::InputInt(u8"经线数", &_uvsphereLongitude, 1, 0);
      recreate |= ImGui::InputInt(u8"纬线数", &_uvsphereLatitude, 1, 0);

      if (recreate) {
        CreateUVSphere();
        _currentMesh = _uvsphere;
      }
    } else if (_currentMeshIndex == 1 || _currentMeshIndex == 2) {
      bool recreate = false;
      recreate = ImGui::InputFloat(u8"球半径", &_sphereRadius, 0.5f, 0.0f);
      recreate |= ImGui::InputInt(u8"细分次数", &_sphereSubdivide, 1, 0);

      if (recreate) {
        if (_currentMeshIndex == 1) {
          CreateNorCubeSphere();
          _currentMesh = _norcubesphere;
        } else if (_currentMeshIndex == 2) {
          CreateIcoSphere();
          _currentMesh = _icosphere;
        }
      }
    }
    ImGui::End();
  }
}

void BasicGeometryView::Destroy() {
  // unbind program
  glUseProgram(0);

  // unbind vao/ebo
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Delete buffers
  glDeleteVertexArrays(20, _vaos);
  glDeleteBuffers(20, _vbos);
  glDeleteBuffers(20, _ebos);

  SAFE_DEL_ARR(_linePoints);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void BasicGeometryView::CreateGeometry(MeshPtr mesh, uint32 index) {
  SubMeshPtr submesh = mesh->GetSubMesh(0);
  // bind vertex array object
  glBindVertexArray(_vaos[index]);

  // bind element array buffer, bind buffer data
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebos[index]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, submesh->GetIndiceNum() * 4,
               submesh->GetIndice(), GL_DYNAMIC_DRAW);

  // bind vertex array buffer, bind buffer data
  glBindBuffer(GL_ARRAY_BUFFER, _vbos[index]);

  glBufferData(GL_ARRAY_BUFFER, mesh->GetVerticeNum() * 12,
               mesh->GetVerticePos(), GL_DYNAMIC_DRAW);
  // vertex attribute layout setting
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
}

void BasicGeometryView::RenderGeometry(MeshPtr mesh, int32 index,
                                       GLenum front_face) {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glFrontFace(front_face);
  if (_polygonLineMode) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  if (_cullFace) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  } else {
    glDisable(GL_CULL_FACE);
  }

  glEnable(GL_BLEND);
  // glBlendFunc(GL_ONE, GL_ONE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // bind shader program
  ShaderProgramUse use_program(_program);

  // shader uniform setting
  glUniformMatrix4fv(_mvpMatLoc, 1, GL_FALSE, glm::value_ptr(_mvpMat));

  SubMeshPtr submesh = mesh->GetSubMesh(0);

  // draw command use vertex array object
  glBindVertexArray(_vaos[index]);
  glDrawElements(GL_TRIANGLES, submesh->GetIndiceNum(), GL_UNSIGNED_INT, NULL);
  // glDrawElements(GL_POINTS, submesh->GetIndiceNum(), GL_UNSIGNED_INT, NULL);

  glFrontFace(GL_CCW);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glDisable(GL_BLEND);
}

/**
 * @brief
 *
 */
void BasicGeometryView::RenderInfinitePlane() {
  MaterialPtr mat = _infinitePlane->GetMeshRenderer()->GetMaterial();
  RenderPassPtr pass = mat->GetRenderPass(0);
  if (pass == nullptr) {
    return;
  }

  ShaderProgramPtr shader = pass->GetShaderProgram();

  for (auto texSlot : pass->GetTextureSlots()) {
    texSlot.second.Apply();
  }
  float aniso = 1.0f;
  glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);

  ShaderProgramUse use(shader);
  for (auto loc : pass->GetUniformLocations()) {
    loc.second.Bind();
  }

  glDisable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glEnable(GL_BLEND);
  // glBlendFunc(GL_ONE, GL_ONE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  SubMeshPtr submesh = _infinitePlane->GetMesh()->GetSubMesh(0);

  // draw command use vertex array object
  glBindVertexArray(_vaos[6]);
  glDrawElements(GL_TRIANGLES, submesh->GetIndiceNum(), GL_UNSIGNED_INT, NULL);

  glDisable(GL_BLEND);

  for (auto texSlot : pass->GetTextureSlots()) {
    texSlot.second.UnBind();
  }
}

/**
 * @brief
 * 直线绘制参考（感谢文章作者）：https://zhuanlan.zhihu.com/p/59541559，文中引文值得仔细阅读。
 * 另外，GPU gems 3（chapter
 * 25：https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch25.html）有一篇关于
 * gpu 绘制向量图的方法介绍。
 */
void BasicGeometryView::RenderLine(MeshPtr mesh, int32 index) {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glEnable(GL_BLEND);
  // glBlendFunc(GL_ONE, GL_ONE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glDisable(GL_CULL_FACE);
  if (_polygonLineMode) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  ShaderProgramUse use_program(_lineProgram);
  {
    glUniformMatrix4fv(_lineMVPLoc, 1, GL_FALSE, glm::value_ptr(_mvpMat));
    glUniform4fv(_lineParametersLoc, 1, glm::value_ptr(_lineParams));
    glUniform4fv(_viewportLoc, 1, glm::value_ptr(_viewportParams));
    glUniform3fv(_lineColorLoc, 1, glm::value_ptr(_lineColor));

    SubMeshPtr submesh = mesh->GetSubMesh(0);

    // draw command use vertex array object
    glBindVertexArray(_vaos[index]);
    glDrawElements(GL_LINES_ADJACENCY, submesh->GetIndiceNum(), GL_UNSIGNED_INT,
                   NULL);
  }

  glDisable(GL_BLEND);
}

/**
 * @brief Render UV Sphere
 *
 */
void BasicGeometryView::RenderUVSphere() {
  RenderGeometry(_uvsphere, 0, GL_CW);
}

void BasicGeometryView::CreateUVSphere() {
  _uvsphere = Mesh::CreateSphereMeshStandard(_sphereRadius, _uvsphereLongitude,
                                             _uvsphereLatitude);
  CreateGeometry(_uvsphere, 0);
}

/**
 * @brief Render Normalized Cube Subdivide Sphere
 *
 */
void BasicGeometryView::RenderNorCubeSphere() {
  RenderGeometry(_norcubesphere, 1, GL_CCW);
}

void BasicGeometryView::CreateNorCubeSphere() {
  _norcubesphere =
      Mesh::CreateSphereMeshNormalizedCube(_sphereRadius, _sphereSubdivide);
  CreateGeometry(_norcubesphere, 1);
}

void BasicGeometryView::RenderIcoSphere() {
  RenderGeometry(_icosphere, 2, GL_CCW);
}

void BasicGeometryView::CreateIcoSphere() {
  _icosphere =
      Mesh::CreateSphereMeshIcosahedron(_sphereRadius, _sphereSubdivide);
  CreateGeometry(_icosphere, 2);
}

void BasicGeometryView::RenderCube() { RenderGeometry(_cube, 3, GL_CCW); }

void BasicGeometryView::CreateCube() {
  _cube = Mesh::CreateCubeMesh(5.0f);
  CreateGeometry(_cube, 3);
}

void BasicGeometryView::RenderQuad() { RenderGeometry(_quad, 4, GL_CCW); }

void BasicGeometryView::CreateQuad() {
  _quad = Mesh::CreateQuadMesh(5.0f, 5.0f);
  CreateGeometry(_quad, 4);
}

void BasicGeometryView::CreateLine() {
  _line = Mesh::CreateLineMesh(_linePoints, 5);
  CreateGeometry(_line, 5);
}

void BasicGeometryView::CreateInfinitePlane() {
  _infinitePlane.reset(
      CreateInifinitePlane(glm::vec3(0.0f, 1.0f, 0.0f), 5.0f, 2.0f, 128));

  ImageManager &imageMgr = ImageMgrInstance();
  std::string imageFilename = {"Assets/Textures/grid.png"};
  ImagePtr image = imageMgr.ReadImage(imageFilename.c_str());

  Texture2DPtr texture = nullptr;
  texture = std::make_shared<Texture2D>(true, true);
  texture->CreateFromImage(image);

  texture->SetWrap<WrapParam::WRAP_S>(WrapMode::REPEAT);
  texture->SetWrap<WrapParam::WRAP_T>(WrapMode::REPEAT);

  texture->SetFilter<FilterParam::MIN_FILTER>(FilterMode::LINEAR_MIPMAP_LINEAR);
  texture->SetFilter<FilterParam::MAG_FILTER>(FilterMode::LINEAR);
  texture->SetAnistropic();

  MaterialPtr material = _infinitePlane->GetMeshRenderer()->GetMaterial();
  material->SetMainTexture(texture);

  CreateGeometry(_infinitePlane->GetMesh(), 6);
}

void BasicGeometryView::CreateQuadBezierLine() {
  glm::vec3 *controlPoints = new glm::vec3[3];
  controlPoints[0] = glm::vec3(-5.f, 0.5f, 0.0f);
  controlPoints[1] = glm::vec3(0.f, 4.0f, 0.0f);
  controlPoints[2] = glm::vec3(5.f, 0.5f, 0.0f);

  _quadbezierline = Mesh::CreateQuadraticBezierLine(controlPoints);
  CreateGeometry(_quadbezierline, 7);

  SAFE_DEL_ARR(controlPoints);
}
void BasicGeometryView::CreateCubicBezierLine() {
  glm::vec3 *controlPoints = new glm::vec3[4];
  controlPoints[0] = glm::vec3(-5.f, 0.5f, 0.0f);
  controlPoints[1] = glm::vec3(-2.5f, 4.0f, 0.0f);
  controlPoints[2] = glm::vec3(2.5f, 0.5f, 0.0f);
  controlPoints[3] = glm::vec3(5.f, 4.0f, 0.0f);

  _cubicbezierline = Mesh::CreateCubicBezierLine(controlPoints);
  CreateGeometry(_cubicbezierline, 8);

  SAFE_DEL_ARR(controlPoints);
}

void BasicGeometryView::CreateBSpline() {
  if (_bspline == nullptr) {
    glm::vec4 *controlPoints = new glm::vec4[5];
    controlPoints[0] = glm::vec4(-5.f, 0.5f, 0.0f, 1.0f);
    controlPoints[1] = glm::vec4(-2.5f, 4.0f, 0.0f, 1.0f);
    controlPoints[2] = glm::vec4(2.5f, 0.5f, 0.0f, 1.0f);
    controlPoints[3] = glm::vec4(5.f, 4.0f, 0.0f, 1.0f);
    controlPoints[4] = glm::vec4(3.f, 8.0f, 0.0f, 1.0f);

    Knot *knots = new Knot[9];
    // knots[0].u = 0.0f;
    // knots[8].u = 1.0f;
    // knots[1].u = 0.1f;
    // knots[2].u = 0.2f;
    // knots[3].u = 0.3f;
    // knots[4].u = 0.4f;
    // knots[5].u = 0.5f;
    // knots[6].u = 0.6f;
    // knots[7].u = 0.8f;

    knots[0].u = 0.0f;
    knots[8].u = 1.0f;
    knots[1].u = 0.0f;
    knots[2].u = 0.0f;
    knots[3].u = 0.0f;
    knots[4].u = 0.4f;
    knots[5].u = 1.0f;
    knots[6].u = 1.0f;
    knots[7].u = 1.0f;

    knots[3].multiplity = 3;
    knots[8].multiplity = 3;

    _bspline = std::make_shared<BSplineCurve>(5, 3, 9, knots, controlPoints);

    MeshPtr mesh = _bspline->GetMesh();
    CreateGeometry(mesh, 9);

    SAFE_DEL_ARR(controlPoints);
    SAFE_DEL_ARR(knots);
  }
}

void BasicGeometryView::CreateNURBS() {}
void BasicGeometryView::CreateBezierSuface() {}

void BasicGeometryView::CreateNURBSSurface() {
  if (_nurbsSurface == nullptr) {
    glm::vec4 *controlPoints = new glm::vec4[25];

    int32 n = 0;
    controlPoints[0] = glm::vec4(-10.f, 0.5f, 0.0f, 1.0f);
    controlPoints[1] = glm::vec4(-8.5f, 3.5f, -3.0f, 1.0f);
    controlPoints[2] = glm::vec4(-8.f, 4.5f, -6.0f, 1.0f);
    controlPoints[3] = glm::vec4(-10.5f, 0.5f, -9.0f, 1.0f);
    controlPoints[4] = glm::vec4(-8.5f, -1.5f, -12.0f, 1.0f);

    // _nurbs_control_line = Mesh::CreateLineMeshEx(controlPoints, 5);
    // CreateGeometry(_nurbs_control_line->GetSubMesh(0), 19);

    n = 5;
    controlPoints[0 + n] = glm::vec4(-10.f + n * 1.0f, 0.0f, 0.0f, 1.0f);
    controlPoints[1 + n] = glm::vec4(-8.5f + n * 1.0f, 5.5f, -3.0f, 1.0f);
    controlPoints[2 + n] = glm::vec4(-8.f + n * 1.0f, 6.5f, -6.0f, 1.0f);
    controlPoints[3 + n] = glm::vec4(-10.5f + n * 1.0f, 0.5f, -9.0f, 1.0f);
    controlPoints[4 + n] = glm::vec4(-8.5f + n * 1.0f, 3.5f, -12.0f, 1.0f);

    n = 10;
    controlPoints[0 + n] = glm::vec4(-10.f + n * 1.0f, 0.0f, 0.0f, 1.0f);
    controlPoints[1 + n] = glm::vec4(-8.5f + n * 1.0f, 5.5f, -3.0f, 1.0f);
    controlPoints[2 + n] = glm::vec4(-8.f + n * 1.0f, 6.5f, -6.0f, 1.0f);
    controlPoints[3 + n] = glm::vec4(-10.5f + n * 1.0f, 0.5f, -9.0f, 1.0f);
    controlPoints[4 + n] = glm::vec4(-8.5f + n * 1.0f, 3.5f, -12.0f, 1.0f);

    n = 15;
    controlPoints[0 + n] = glm::vec4(-10.f + n * 1.0f, 0.0f, 0.0f, 1.0f);
    controlPoints[1 + n] = glm::vec4(-8.5f + n * 1.0f, 5.5f, -3.0f, 1.0f);
    controlPoints[2 + n] = glm::vec4(-8.f + n * 1.0f, 6.5f, -6.0f, 1.0f);
    controlPoints[3 + n] = glm::vec4(-10.5f + n * 1.0f, 0.5f, -9.0f, 1.0f);
    controlPoints[4 + n] = glm::vec4(-8.5f + n * 1.0f, 3.5f, -12.0f, 1.0f);

    n = 20;
    controlPoints[0 + n] = glm::vec4(-10.f + n * 1.0f, 0.0f, 0.0f, 1.0f);
    controlPoints[1 + n] = glm::vec4(-8.5f + n * 1.0f, 5.5f, -3.0f, 1.0f);
    controlPoints[2 + n] = glm::vec4(-8.f + n * 1.0f, 6.5f, -6.0f, 1.0f);
    controlPoints[3 + n] = glm::vec4(-10.5f + n * 1.0f, 0.5f, -9.0f, 1.0f);
    controlPoints[4 + n] = glm::vec4(-8.5f + n * 1.0f, 3.5f, -12.0f, 1.0f);

    Knot *uKnots = new Knot[9];
    uKnots[0].u = 0.0f;
    uKnots[8].u = 1.0f;
    uKnots[1].u = 0.0f;
    uKnots[2].u = 0.0f;
    uKnots[3].u = 0.0f;
    uKnots[4].u = 0.5f;
    uKnots[5].u = 1.f;
    uKnots[6].u = 1.f;
    uKnots[7].u = 1.f;

    uKnots[3].multiplity = 3;
    uKnots[8].multiplity = 3;

    Knot *vKnots = new Knot[9];
    vKnots[0].u = 0.0f;
    vKnots[8].u = 1.0f;
    vKnots[1].u = 0.0f;
    vKnots[2].u = 0.0f;
    vKnots[3].u = 0.0f;
    vKnots[4].u = 0.5f;
    vKnots[5].u = 1.f;
    vKnots[6].u = 1.f;
    vKnots[7].u = 1.f;

    vKnots[3].multiplity = 3;
    vKnots[8].multiplity = 3;

    _nurbsSurface = std::make_shared<NURBSSurface>(5, 3, 5, 3, controlPoints,
                                                   uKnots, vKnots);

    // _nurbsSurface->SetControlPoints(controlPoints, 25);
    // _nurbsSurface->SetUKnots(uKnots, 9);
    // _nurbsSurface->SetVKnots(vKnots, 9);
    // _nurbsSurface->GenerateMeshInternal();

    MeshPtr mesh = _nurbsSurface->GetMesh();
    CreateGeometry(mesh, 10);

    SAFE_DEL_ARR(controlPoints);
    SAFE_DEL_ARR(uKnots);
    SAFE_DEL_ARR(vKnots);
  }
}

} // namespace TwinkleGraphics
