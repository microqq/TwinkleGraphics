

#include "imgui.h"

#include "twAntiAliasing.h"
#include "twImage.h"
#include "twImageManager.h"
#include "twMaterialInstance.h"
#include "twModelManager.h"
#include "twRenderTexture.h"
#include "twConsoleLog.h"
#include "twImGuiContextManager.h"

namespace TwinkleGraphics {
AntiAliasing::AntiAliasing(std::string &name)
    : GLViewPlugin(name), _view(nullptr)
// , _view2(nullptr)
{}

AntiAliasing::~AntiAliasing() {
  SAFE_DEL(_view);
  // SAFE_DEL(_view2);
}

void AntiAliasing::Install() {
  Plugin::Install();

  // create view
  _view = new AntiAliasingView();
  // _view2 = new AntiAliasingView();

  Viewport viewport(Rect(0, 0, 1024, 768), 17664U, RGBA(0.0f, 0.f, 0.0f, 1.f));
  CameraPtr camera = std::make_shared<Camera>(viewport, 45.0f, 0.1f, 10000.0f);
  _view->SetViewCamera(camera);

  FirstPersonControlPtr cameraControl =
      std::make_shared<FirstPersonControl>(camera);
  cameraControl->SetMinDistance(-5000.0f);
  cameraControl->SetMaxDistance(5000.0f);
  cameraControl->SetStep(2.0f, 2.0f);
  _view->SetCameraControl(cameraControl);

  AntiAliasingScene::Ptr scene = std::make_shared<AntiAliasingScene>();
  scene->SetMainCamera(camera);
  _view->AttachScene(scene);

  _views[_viewsCount++] = _view;
  // _views[_views_count++] = _view2;
}

void AntiAliasing::UnInstall() {
  _view->Destroy();
  SAFE_DEL(_view);

  Plugin::UnInstall();
}

void AntiAliasing::UpdateViews() {
  // const glm::ivec2& size = _view->GetViewSize();
  // Rect rect(0, 0, size.x, size.y);
  // _view->ResetViewRect(rect);
  // float32 ratio = (float32)size.x / (float32)size.y;
  // int32 w = ratio * 200.0f;
  // _view2->ResetViewport(Rect(size.x - w, 0, w, 200.0f));
}

void AntiAliasingView::Initialized() {
  if (_initialized)
    return;
  if (_scene != nullptr) {
    _scene->Init();
  }

  View::Initialized();
}

void AntiAliasingView::Destroy() {
  dynamic_cast<AntiAliasingScene *>(_scene.get())->DestroyScene();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void AntiAliasingView::OnGUI() {
  // TwinkleGraphics::MainMenuBar();

  AntiAliasingScene *scene = dynamic_cast<AntiAliasingScene *>(_scene.get());
  int32 &currentAAOption = scene->GetCurrentAAOption();
  int32 &currentMSAASWOption = scene->_currentMSAASWOption;
  int32 &currentRSFOption = scene->_resolveFitlerOption;

  ImGuiContextManager &imguiCtxMgr = ImGuiContextMgrInstance();
  imguiCtxMgr.SetCurrentContext();
  ImGui::Begin(u8"反走样");
  {

    ImGui::RadioButton(u8"NO AA", &(currentAAOption), AAOption::NONE);
    ImGui::RadioButton(u8"MSAA_HW", &(currentAAOption), AAOption::MSAA_HW);
    ImGui::RadioButton(u8"MSAA_SW", &currentAAOption, AAOption::MSAA_SW);
    if (currentAAOption == AAOption::MSAA_SW) {
      ImGui::BeginGroup();
      ImGui::Indent();
      ImGui::Checkbox(u8"多重采样", &(scene->_enableMultisample));
      ImGui::RadioButton(u8"Back Buffer", &(currentMSAASWOption),
                         MSAASWOption::MSAA_SW_BACKBUFFER);
      ImGui::RadioButton(u8"Screen FBO", &(currentMSAASWOption),
                         MSAASWOption::MSAA_SW_SCREENFBO);
      ImGui::RadioButton(u8"Resolve Filter", &(currentMSAASWOption),
                         MSAASWOption::MSAA_SW_RESOLVE);
      if (currentMSAASWOption == MSAASWOption::MSAA_SW_RESOLVE) {
        ImGui::Indent();
        ImGui::Combo("", &currentRSFOption,
                     "Box\0Tiangle\0Cubic\0Hann and Hamming Window\0Blackman "
                     "Window\0Kaiser Window\0"
                     "Lanczos3 Window\0Lanczos4 Window\0Lanczos6 Window\0\0");
        ImGui::Unindent();

        if (currentRSFOption >= ResolveFilterOption::BOX) {
          MaterialPtr resolve_mat = scene->_msaaResolveMat;
          resolve_mat->SetSimpleUniformValue<int, 1>("filterOption",
                                                     currentRSFOption);
          // set resolve filter option
        }

        MeshRendererPtr renderer = scene->_screenQuadMSAA->GetMeshRenderer();
        renderer->SetMaterial(scene->_msaaResolveMat);
        scene->_msaaResolveMat = renderer->GetMaterial();
      } else {
        MeshRendererPtr renderer = scene->_screenQuadMSAA->GetMeshRenderer();
        renderer->SetMaterial(scene->_screenQuadMat);
        scene->_screenQuadMat = renderer->GetMaterial();
      }
      ImGui::EndGroup();
    }

    ImGui::RadioButton(u8"SSAA", &currentAAOption, AAOption::SSAA);
    ImGui::RadioButton(u8"CSAA", &currentAAOption, AAOption::CSAA);
    ImGui::RadioButton(u8"CFAA", &currentAAOption, AAOption::CFAA);
    ImGui::RadioButton(u8"FXAA", &currentAAOption, AAOption::FXAA);
  }
  ImGui::End();

  imguiCtxMgr.SetCurrentContext();
  ImGui::Begin(u8"Window");
  {
    ImGui::BeginChild(u8"SubWindow", ImVec2(512.0f, 512.0f), false,
                      ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
                          ImGuiWindowFlags_::ImGuiWindowFlags_NoMove);
    {
      TexturePtr rt = scene->_rtScreen->GetTexture();
      uint id = rt->GetRenderRes().id;
      ImGui::Image((ImTextureID)id, ImVec2(512, 512), ImVec2(0.0f, 1.0f),
                   ImVec2(1.0f, 0.0f));
    }
    ImGui::EndChild();
  }
  ImGui::End();

  imguiCtxMgr.SetCurrentContext();
  Select3DModel();
}

void AntiAliasingView::Select3DModel() {
  ImGui::Begin(u8"选择模型");
  FileDialogPanel(".obj,.OBJ,.fbx,.FBX,.3ds,.3DS,.gltf", _selectFileInfo);
  if (_selectFileInfo.selectChanged) {
    AntiAliasingScene *scene = dynamic_cast<AntiAliasingScene *>(_scene.get());
    scene->Load3DModel(_selectFileInfo.filePathName);

    _selectFileInfo.selectChanged = false;
  }
  ImGui::End();

  // std::cout << _selectFileInfo.filePathName << std::endl;
  // std::cout << _selectFileInfo.filePath << std::endl;
  // std::cout << _selectFileInfo.fileName << std::endl;
}

void AntiAliasingScene::CreateScene() {
  // create conel box
  _planeLeft.reset(CreatePlane(glm::vec3(1.0f, 0.0f, 0.0f), 30.0f));
  TransformPtr trans = _planeLeft->GetTransform();
  {
    trans->Translate(glm::vec3(-15.0f, 0.0f, 0.0f));
    CreateGeometry(_planeLeft, 0);
  }

  _planeTop.reset(CreatePlane(glm::vec3(0.0f, -1.0f, 0.0f), 30.0f));
  {
    trans = _planeTop->GetTransform();
    trans->Translate(glm::vec3(0.0f, 15.0f, 0.0f));
    CreateGeometry(_planeTop, 1);
  }

  _planeRight.reset(CreatePlane(glm::vec3(1.0f, 0.0f, 0.0f), 30.0f));
  {
    trans = _planeRight->GetTransform();
    trans->Translate(glm::vec3(15.0f, 0.0f, 0.0f));
    CreateGeometry(_planeRight, 2);
  }

  _planeBottom.reset(CreatePlane(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f));
  {
    trans = _planeBottom->GetTransform();
    trans->Translate(glm::vec3(0.0f, -15.0f, 0.0f));
    CreateGeometry(_planeBottom, 3);
  }

  _planeBack.reset(CreatePlane(glm::vec3(0.0f, 0.0f, 1.0f), 30.0f));
  {
    trans = _planeBack->GetTransform();
    trans->Translate(glm::vec3(0.0f, 0.0f, -15.0f));
    CreateGeometry(_planeBack, 4);
  }

  _rootNode = std::make_shared<SceneNode>();
  _rootTrans = _rootNode->GetTransform();

  // icosphere
  _sphere.reset(
      CreateIcosahedronSphere(3.0f, 10, MeshDataFlag(MeshDataFlag::HAS_UV)));
  {
    trans = _sphere->GetTransform();
    trans->Translate(glm::vec3(-5.0f, -5.0f, 0.0f));
    _rootNode->AddChild(_sphere);

    CreateGeometry(_sphere, 5);

    ImageManager &imageMgr = ImageMgrInstance();
    std::string imageFilename{"Assets/Textures/TantolundenCube.dds"};
    ImagePtr image = imageMgr.ReadImage(imageFilename.c_str());
    TextureCubePtr cubemap = std::make_shared<TextureCube>(true);
    cubemap->CreateFromImage(image);

    MaterialPtr mat = _sphere->GetMeshRenderer()->GetMaterial();
    mat->SetMainTexture(cubemap);
  }

  // cube
  _cube.reset(CreateCube(5.0f));
  {
    trans = _cube->GetTransform();
    trans->Translate(glm::vec3(5.0f, -5.0f, 0.0f));
    _rootNode->AddChild(_cube);

    CreateGeometry(_cube, 6);
  }

  // triangles
  glm::vec4 tintcolor(0.6f, 0.9f, 0.2f, 1.0f);
  glm::vec3 p0(-3.0f, 5.0f, 0.0f), p1(-4.0f, 0.0f, 0.0f), p2(3.0f, 5.2f, 0.0f);
  _triangleBack.reset(CreateTriangle(p0, p1, p2));
  {
    MaterialPtr material = _triangleBack->GetMeshRenderer()->GetMaterial();
    material->SetVecUniformValue<float32, 4>("tintColor", tintcolor);

    CreateGeometry(_triangleBack, 7);
  }
  p0 = glm::vec3(-2.0f, 3.0f, 0.0f);
  p1 = glm::vec3(-3.0f, 0.0f, 0.0f);
  p2 = glm::vec3(3.0f, 2.3f, 0.0f);
  _triangleFront.reset(CreateTriangle(p0, p1, p2));
  {
    tintcolor = glm::vec4(1.0f, 0.3f, 0.8f, 1.0f);
    MaterialPtr material = _triangleFront->GetMeshRenderer()->GetMaterial();
    material->SetVecUniformValue<float32, 4>("tintColor", tintcolor);

    CreateGeometry(_triangleFront, 8);
  }

  // rt for msaa resolve fitler
  _rtMSAA = std::make_shared<RenderTexture>(1024, 768, GL_RGBA8, GL_RGBA, true,
                                            false, true, _numSamples, true);
  _rtMSAA->Create(nullptr);

  _rtScreen = std::make_shared<RenderTexture>(1024, 768, GL_RGBA8, GL_RGBA,
                                              false, false, false, 1, false);
  _rtScreen->Create(nullptr);
  _rtScreen->GetTexture()->SetFilter<FilterParam::MIN_FILTER>(
      FilterMode::LINEAR);
  _rtScreen->GetTexture()->SetFilter<FilterParam::MAG_FILTER>(
      FilterMode::LINEAR);

  vec2 size(2, 2);
  _screenQuadMSAA = std::make_shared<Quad>(size, MeshDataFlag(8));
  // _screenQuadMSAA->GenerateMeshInternal();
  { CreateGeometry(_screenQuadMSAA, 9); }

  /**
   *
   * Debug Mode With GDB:
   *  Call ReadShaderAsync in plugin Antialiasing, call function(future.get())
   *  would throw exception that unknown/just-in-time compiled code.
   *
   *  So, fix this problem with solution that makes the operations of
   * _futures.push & future.get() in Common Update.
   */
  // ShaderOption option(
  //     ShaderOption::OptionData{std::string("Assets/Shaders/cube.frag"),
  //     ShaderType::FRAGMENT_SHADER});
  // shaderMgr.ReadShaderAsync("Assets/Shaders/cube.frag", &option);

  MeshRendererPtr quadRenderer = std::make_shared<MeshRenderer>();
  _screenQuadMat = std::make_shared<ScreenQuadMaterial>();
  quadRenderer->SetMaterial(_screenQuadMat);

  _screenQuadMat = quadRenderer->GetMaterial();
  _screenQuadMat->SetMainTexture(_rtScreen->GetTexture());

  quadRenderer->SetMesh(_screenQuadMSAA->GetMesh());
  _screenQuadMSAA->SetMeshRenderer(quadRenderer);

  // create material for custom msaa resolve filter
  _msaaResolveMat = std::make_shared<MSAAResolveMaterial>();
  _msaaResolveMat->SetMainTexture(_rtMSAA->GetTexture());
  _msaaResolveMat->SetSimpleUniformValue<int, 1>("samplecount", _numSamples);

  CreateSkybox();
  // CreateInfinitePlane();
}

void AntiAliasingScene::Load3DModel(std::string filename) {
  ModelManager &modelMgr = ModelMgrInstance();
  ReaderOption *option = new ReaderOption;
  ReadSuccessCallbackFuncPtr funcPtr =
      std::make_shared<ReadSuccessCallbackFunc>([this](ObjectPtr obj) {
        if (obj != nullptr) {
          _model = std::dynamic_pointer_cast<Model>(obj);
          _model->SetValid(true);
        }
      });
  option->AddSuccessFunc(-1, funcPtr);
  modelMgr.ReadModelAsync(filename.c_str(), option);

  // _model = modelMgr.ReadModel(filename.c_str());
  // _model = modelMgr->ReadModel("Assets/Models/Sponza/sponza.obj");
  // _sponza = modelMgr->ReadModel("Assets/Models/bunny.obj");
  // _sponza = modelMgr->ReadModel("Assets/Models/Armadillo/Armadillo.ply");
}

void AntiAliasingScene::UpdateScene() {
  // CameraPtr camera = _current_aa_option == AAOption::MSAA_SW ?
  //     _msaa_camera : _maincamera;
  CameraPtr camera = _maincamera;
  const Viewport &viewport = camera->GetViewport();

  _viewportParams =
      glm::vec4((float32)(viewport.Width()), (float32)(viewport.Height()),
                viewport.AspectRatio(), 1.0f);
  _viewMat = camera->GetViewMatrix();
  _projectionMat = camera->GetProjectionMatrix();
  _mvpMat = _projectionMat * _viewMat;

  MaterialPtr skyboxmat = _skybox->GetMeshRenderer()->GetMaterial();
  mat4 rotateMat = glm::mat4_cast(camera->GetOrientation());
  // mat4 mvp = _projection_mat * glm::mat4(glm::mat3(_view_mat));
  mat4 mvp = _projectionMat * rotateMat;
  skyboxmat->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);

  // MaterialPtr infplaneMaterial =
  // _infinitePlane->GetMeshRenderer()->GetSharedMaterial();
  // infplaneMaterial->SetMatrixUniformValue<float32, 4, 4>("mvp", _mvpMat);

  MaterialPtr materialPlaneLeft =
      _planeLeft->GetMeshRenderer()->GetMaterial();
  glm::mat4 model = _planeLeft->GetTransform()->GetLocalToWorldMatrix();
  mvp = _mvpMat * model;
  materialPlaneLeft->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
  glm::vec4 tintColor(0.7f, 0.0f, 0.0f, 1.0f);
  materialPlaneLeft->SetVecUniformValue<float32, 4>("tintColor", tintColor);

  MaterialPtr materialPlaneTop = _planeTop->GetMeshRenderer()->GetMaterial();
  {
    model = _planeTop->GetTransform()->GetLocalToWorldMatrix();
    mvp = _mvpMat * model;
    materialPlaneTop->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
    tintColor = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
    materialPlaneTop->SetVecUniformValue<float32, 4>("tintColor", tintColor);
  }

  MaterialPtr materialPlaneRight =
      _planeRight->GetMeshRenderer()->GetMaterial();
  {
    model = _planeRight->GetTransform()->GetLocalToWorldMatrix();
    mvp = _mvpMat * model;
    materialPlaneRight->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
    tintColor = glm::vec4(0.0f, 0.7f, 0.0f, 1.0f);
    materialPlaneRight->SetVecUniformValue<float32, 4>("tintColor",
                                                         tintColor);
  }

  MaterialPtr materialPlaneBottom =
      _planeBottom->GetMeshRenderer()->GetMaterial();
  {
    model = _planeBottom->GetTransform()->GetLocalToWorldMatrix();
    mvp = _mvpMat * model;
    materialPlaneBottom->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
    tintColor = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
    materialPlaneBottom->SetVecUniformValue<float32, 4>("tintColor",
                                                          tintColor);
  }

  MaterialPtr materialPlaneBack =
      _planeBack->GetMeshRenderer()->GetMaterial();
  {
    model = _planeBack->GetTransform()->GetLocalToWorldMatrix();
    tintColor = glm::vec4(0.7f, 0.7f, 0.0f, 1.0f);
    mvp = _mvpMat * model;
    materialPlaneBack->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
    materialPlaneBack->SetVecUniformValue<float32, 4>("tintColor", tintColor);
  }

  MaterialPtr materialSphere = _sphere->GetMeshRenderer()->GetMaterial();
  {
    TransformPtr trans = _sphere->GetTransform();
    trans->Rotate(0.002f, glm::vec3(0.0f, 1.0f, 0.0f));
    mvp = _mvpMat * trans->GetLocalToWorldMatrix();
    materialSphere->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);

    TexturePtr spheretex = materialSphere->GetMainTexture();
    spheretex->SetWrap<WrapParam::WRAP_S>(WrapMode::CLAMP_TO_EDGE);
    spheretex->SetWrap<WrapParam::WRAP_T>(WrapMode::CLAMP_TO_EDGE);
    spheretex->SetWrap<WrapParam::WRAP_R>(WrapMode::CLAMP_TO_EDGE);

    spheretex->SetFilter<FilterParam::MIN_FILTER>(
        FilterMode::LINEAR_MIPMAP_LINEAR);
    spheretex->SetFilter<FilterParam::MAG_FILTER>(FilterMode::LINEAR);
  }

  MaterialPtr materialCube = _cube->GetMeshRenderer()->GetMaterial();
  {
    TransformPtr trans = _cube->GetTransform();
    trans->Rotate(0.002f, glm::vec3(0.0f, 1.0f, 0.0f));

    mvp = _mvpMat * trans->GetLocalToWorldMatrix();
    materialCube->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
  }

  MaterialPtr materialTriangle =
      _triangleBack->GetMeshRenderer()->GetMaterial();
  { materialTriangle->SetMatrixUniformValue<float32, 4, 4>("mvp", _mvpMat); }
  materialTriangle = _triangleFront->GetMeshRenderer()->GetMaterial();
  { materialTriangle->SetMatrixUniformValue<float32, 4, 4>("mvp", _mvpMat); }

  _rootTrans->Rotate(0.001f, glm::vec3(0.0f, 1.0f, 0.0f));

  // update sponza -- just for test
  if (_model != nullptr) {
    int32 count = _model->GetGeometriesCount();
    for (int32 i = 0; i < count; i++) {
      GeometryPtr geo = _model->GetGeometry(i);
      MeshRendererPtr geoRenderer = geo->GetMeshRenderer();
      if (geoRenderer == nullptr) {
        continue;
      }

      model = geo->GetTransform()->GetLocalToWorldMatrix();
      mvp = _mvpMat * model;

      for (int32 k = 0, num = geoRenderer->GetMaterialCount(); k < num; k++) {
        MaterialPtr geoMat = geoRenderer->GetMaterial(k);
        geoMat->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
        glm::vec4 tintColor(0.6f, 0.6f, 0.6f, 1.0f);
        geoMat->SetVecUniformValue<float32, 4>("tintColor", tintColor);
      }
    }
  }
}

void AntiAliasingScene::RenderScene() {
  switch (_currentAAOption) {
  case AAOption::MSAA_HW:
    _maincamera->ClearRenderContext();
    glEnable(GL_MULTISAMPLE);
    RenderGeometrys();
    glDisable(GL_MULTISAMPLE);
    break;
  case AAOption::MSAA_SW:
    _rtMSAA->Bind();
    {
      // render with msaa fbo
      // _msaa_camera->ClearRenderContext();
      _maincamera->ClearRenderContext();
      if (_enableMultisample)
        glEnable(GL_MULTISAMPLE);
      else
        glDisable(GL_MULTISAMPLE);
      RenderGeometrys();
      if (_enableMultisample)
        glDisable(GL_MULTISAMPLE);

      // blit to backbuffer
      if (_currentMSAASWOption == MSAASWOption::MSAA_SW_BACKBUFFER) {
        _rtMSAA->BlitColorToBackBuffer(1024, 768);
      }
      // blit to screen fbo
      else if (_currentMSAASWOption == MSAASWOption::MSAA_SW_SCREENFBO) {
        _rtMSAA->BlitColor(_rtScreen);
      }
    }
    _rtMSAA->UnBind();

    // render screen quad
    if (_currentMSAASWOption == MSAASWOption::MSAA_SW_SCREENFBO ||
        _currentMSAASWOption == MSAASWOption::MSAA_SW_RESOLVE) {
      _maincamera->ClearRenderContext();
      RenderScreenQuad();
    }
    break;
  default:
    _maincamera->ClearRenderContext();
    glDisable(GL_MULTISAMPLE);
    RenderGeometrys();
    break;
  }
}

void AntiAliasingScene::RenderGeometrys() {
  RenderSkybox();
  // RenderInfinitePlane();

  // RenderGeometry(_planeLeft, 0);
  // RenderGeometry(_planeTop, 1);
  // RenderGeometry(_planeRight, 2);
  // RenderGeometry(_planeBottom, 3);
  // RenderGeometry(_planeBack, 4);
  // RenderGeometry(_sphere, 5);
  // RenderGeometry(_cube, 6);
  // RenderGeometry(_triangleBack, 7);

  // glEnable(GL_POLYGON_OFFSET_FILL);
  // glPolygonOffset(-1.0f, 0.0f);
  // RenderGeometry(_triangleFront, 8);
  // glDisable(GL_POLYGON_OFFSET_FILL);

  // just for test
  if (_model != nullptr) {
    _model->Render();
  }
}

void AntiAliasingScene::RenderScreenQuad() {
  RenderGeometry(_screenQuadMSAA, 9);
}

void AntiAliasingScene::RenderSkybox() { RenderGeometry(_skybox, 10); }

void AntiAliasingScene::RenderInfinitePlane() {
  // glEnable(GL_MULTISAMPLE);
  RenderGeometry(_infinitePlane, 11);
  // glDisable(GL_MULTISAMPLE);
}

void AntiAliasingScene::ResizeScreenTexture() {}

void AntiAliasingScene::DestroyScene() {
  ShaderProgramUse use(nullptr);

  // unbind vao/ebo
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Delete buffers
  glDeleteVertexArrays(16, _vaos);
  glDeleteBuffers(16, _vbos);
  glDeleteBuffers(16, _ebos);
}

void AntiAliasingScene::CreateGeometry(GeometryPtr geom, uint32 index) {
  MeshPtr mesh = geom->GetMesh();
  SubMeshPtr submesh = mesh->GetSubMesh(0);

  // bind vertex array object
  glBindVertexArray(_vaos[index]);

  // bind element array buffer, bind buffer data
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebos[index]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, submesh->GetIndiceNum() * 4,
               submesh->GetIndice(), GL_DYNAMIC_DRAW);

  // bind vertex array buffer, bind buffer data
  glBindBuffer(GL_ARRAY_BUFFER, _vbos[index]);
  int32 verticenum = mesh->GetVerticeNum();
  if (geom->GetMeshDataFlag() == MeshDataFlag::DEFAULT) {
    glBufferData(GL_ARRAY_BUFFER, mesh->GetVerticeNum() * 12,
                 mesh->GetVerticePos(), GL_DYNAMIC_DRAW);
  } else {
    // map vertexbuffer data(position & uv)
    glBufferData(GL_ARRAY_BUFFER, mesh->GetDataSize(), nullptr,
                 GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, verticenum * 12, mesh->GetVerticePos());

    if ((geom->GetMeshDataFlag() & MeshDataFlag::HAS_UV) != 0)
      glBufferSubData(GL_ARRAY_BUFFER, verticenum * 12, verticenum * 16,
                      mesh->GetVerticeUV());
  }

  // bind vertex position
  glEnableVertexAttribArray(0);
  glBindVertexBuffer(0, _vbos[index], 0, sizeof(vec3));
  glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexAttribBinding(0, 0);

  // bind vertex uv
  if ((geom->GetMeshDataFlag() & MeshDataFlag::HAS_UV) != 0) {
    glEnableVertexAttribArray(4);
    glBindVertexBuffer(1, _vbos[index], verticenum * 12, sizeof(vec4));
    glVertexAttribFormat(4, 4, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(4, 1);
  }

  glBindVertexArray(0);
}

void AntiAliasingScene::CreateSkybox() {
  if (_skybox == nullptr) {
    _skybox = std::make_shared<Cube>(2.0f, MeshDataFlag::DEFAULT);
    // _skybox->GenerateMeshInternal();

    MeshRendererPtr renderer = std::make_shared<MeshRenderer>();
    MaterialPtr mat = std::make_shared<SkyboxMaterial>();

    TextureCubePtr cubemap = std::make_shared<TextureCube>(true);
    cubemap->SetWrap<WrapParam::WRAP_S>(WrapMode::CLAMP_TO_EDGE);
    cubemap->SetWrap<WrapParam::WRAP_T>(WrapMode::CLAMP_TO_EDGE);
    cubemap->SetWrap<WrapParam::WRAP_R>(WrapMode::CLAMP_TO_EDGE);

    cubemap->SetFilter<FilterParam::MIN_FILTER>(
        FilterMode::LINEAR_MIPMAP_LINEAR);
    cubemap->SetFilter<FilterParam::MAG_FILTER>(FilterMode::LINEAR);

    ImageManager &imageMgr = ImageMgrInstance();
    std::string frontInfo = {"Assets/Textures/skybox/front.png"};
    ImagePtr frontImage = imageMgr.ReadImage(frontInfo.c_str());
    std::string backInfo = {"Assets/Textures/skybox/back.png"};
    ImagePtr backImage = imageMgr.ReadImage(backInfo.c_str());
    std::string leftInfo = {"Assets/Textures/skybox/left.png"};
    ImagePtr leftImage = imageMgr.ReadImage(leftInfo.c_str());
    std::string rightInfo = {"Assets/Textures/skybox/right.png"};
    ImagePtr rightImage = imageMgr.ReadImage(rightInfo.c_str());
    std::string topInfo = {"Assets/Textures/skybox/top.png"};
    ImagePtr topImage = imageMgr.ReadImage(topInfo.c_str());
    std::string downInfo = {"Assets/Textures/skybox/bottom.png"};
    ImagePtr downImage = imageMgr.ReadImage(downInfo.c_str());

    cubemap->SetPositiveX(rightImage);
    cubemap->SetPositiveY(topImage);
    cubemap->SetPositiveZ(frontImage);
    cubemap->SetNegativeX(leftImage);
    cubemap->SetNegativeY(downImage);
    cubemap->SetNegativeZ(backImage);
    cubemap->InitStorageByOthers();

    // std::string imageFilename = {"Assets/Textures/skybox/output.dds"};
    // ImagePtr image = imageMgr.ReadImage(imageFilename.c_str());
    // cubemap->CreateFromImage(image);

    renderer->SetMaterial(mat);
    mat = renderer->GetMaterial();
    mat->SetMainTexture(cubemap);

    renderer->SetMesh(_skybox->GetMesh());
    _skybox->SetMeshRenderer(renderer);

    CreateGeometry(_skybox, 10);
  }
}

void AntiAliasingScene::CreateInfinitePlane() {
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

  MaterialPtr material = _infinitePlane->GetMeshRenderer()->GetMaterial();
  material->SetMainTexture(texture);

  CreateGeometry(_infinitePlane, 11);
}

void AntiAliasingScene::RenderGeometry(GeometryPtr geom, int32 index) {
  MaterialPtr mat = geom->GetMeshRenderer()->GetMaterial();
  RenderPassPtr pass = mat->GetRenderPass(0);
  if (pass == nullptr) {
    return;
  }

  for (auto texSlot : pass->GetTextureSlots()) {
    texSlot.second.Apply();
  }

  ShaderProgramPtr shader = pass->GetShaderProgram();
  ShaderProgramUse use(shader);
  for (auto loc : pass->GetUniformLocations()) {
    loc.second.Bind();
  }

  glFrontFace(GL_CCW);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glDisable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  SubMeshPtr submesh = geom->GetMesh()->GetSubMesh(0);

  // draw command use vertex array object
  glBindVertexArray(_vaos[index]);
  glDrawElements(GL_TRIANGLES, submesh->GetIndiceNum(), GL_UNSIGNED_INT, NULL);

  for (auto texSlot : pass->GetTextureSlots()) {
    texSlot.second.UnBind();
  }
}

} // namespace TwinkleGraphics
