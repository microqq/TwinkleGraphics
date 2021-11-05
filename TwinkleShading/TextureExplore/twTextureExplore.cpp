

#include "imgui.h"
// #include "imgui_filebrowser.h"
#include "twImage.h"
#include "twImageManager.h"
#include "twMaterialInstance.h"
#include "twTextureExplore.h"

namespace TwinkleGraphics {
TextureExplore::TextureExplore(std::string &name)
    : GLPlugin(name), _view(nullptr) {}

TextureExplore::~TextureExplore() { SAFE_DEL(_view); }

void TextureExplore::Install() {
  Plugin::Install();

  // Initilize view
  _view = new TextureExploreView();
  _views[_viewsCount++] = _view;
}

void TextureExplore::UnInstall() {
  _view->Destroy();
  SAFE_DEL(_view);

  Plugin::UnInstall();
}

void TextureExploreView::Initialize() {
  if (_initialized)
    return;

  Viewport viewport(Rect(0, 0, _rect.z, _rect.w), 17664U,
                    RGBA(0.0f, 0.f, 0.f, 1.f));
  CameraPtr camera = std::make_shared<Camera>(viewport, 45.0f, 0.1f, 5000.0f);
  this->SetViewCamera(camera);

  FirstPersonControlPtr cameraControl =
      std::make_shared<FirstPersonControl>(camera);
  cameraControl->SetMinDistance(-5000.0f);
  cameraControl->SetMaxDistance(5000.0f);
  cameraControl->SetDistance(25.0f);
  this->SetCameraControl(cameraControl);

  // create vertex array object
  _vaos = new uint32[16];
  glGenVertexArrays(16, _vaos);

  // create vertex buffer object
  _vbos = new uint32[16];
  glGenBuffers(16, _vbos);
  _ebos = new uint32[16];
  glGenBuffers(16, _ebos);

  Viewport pro_viewport(Rect(0, 0, _rect.z, _rect.w), 17664U,
                        RGBA(0.0f, 0.f, 0.f, 1.f));
  _projTexCamera = std::make_shared<Camera>(pro_viewport, 30.0f, 0.1f, 1000.0f);
  // _proj_tex_camera->SetOrientation(quat(1.0f, 0.0f, 0.0f, glm::pi<float32>()
  // * 0.5f));
  _projTexCamera->Translate(vec3(0.0f, 0.0f, 10.0f));

  View::Initialize();
}

void TextureExploreView::Destroy() {
  ShaderProgramUse use(nullptr);

  // unbind vao/ebo
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Delete buffers
  glDeleteVertexArrays(16, _vaos);
  glDeleteBuffers(16, _vbos);
  glDeleteBuffers(16, _ebos);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void TextureExploreView::Advance(float64 delta_time) {
  View::Advance(delta_time);

  const Viewport &viewport = _camera->GetViewport();
  _viewportParams =
      glm::vec4((float32)(viewport.Width()), (float32)(viewport.Height()),
                viewport.AspectRatio(), 1.0f);
  _viewMat = _camera->GetViewMatrix();
  _projectionMat = _camera->GetProjectionMatrix();
  _mvpMat = _projectionMat * _viewMat;

  // sprite setting
  {
    SpritePtr sprite = nullptr;
    if (_currentTexOption == 0)
      sprite = _sprite1D;
    else if (_currentTexOption == 1)
      sprite = _sprite;

    if (sprite != nullptr) {
      MaterialPtr mat = nullptr;
      mat = sprite->GetMaterial();
      mat->SetMatrixUniformValue<float32, 4, 4>("mvp", _mvpMat);
      vec4 tintColor(_tintcolor[0], _tintcolor[1], _tintcolor[1],
                     _tintcolor[3]);
      mat->SetVecUniformValue<float32, 4>("tintColor", tintColor);
      mat->SetTextureTiling("mainTex", _texTiling);
      mat->SetTextureOffset("mainTex", _texOffset);

      // set sprite texture parameters
      TexturePtr tex = sprite->GetTexture();
      tex->SetWrap<WrapParam::WRAP_S>(_texparams.wrapModes[0]);
      if (sprite == _sprite)
        tex->SetWrap<WrapParam::WRAP_T>(_texparams.wrapModes[1]);

      tex->SetFilter<FilterParam::MIN_FILTER>(_texparams.filterModes[0]);
      tex->SetFilter<FilterParam::MAG_FILTER>(_texparams.filterModes[1]);

      tex->SetTexBorderColor(_texparams.bordercolorParameter,
                             _texparams.borderColor);
    }
  }
  // skybox setting
  {
    GeometryPtr geom = nullptr;
    if (_currentTexOption == 3 || _enableSkybox) {
      geom = _skybox;
      MaterialPtr skyboxmat = geom->GetMeshRenderer()->GetMaterial();
      mat4 rotate_mat = glm::mat4_cast(_camera->GetOrientation());
      // mat4 mvp = _projection_mat * glm::mat4(glm::mat3(_view_mat));
      mat4 mvp = _projectionMat * rotate_mat;

      skyboxmat->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);

      TexturePtr skyboxtex = skyboxmat->GetMainTexture();
      skyboxtex->SetWrap<WrapParam::WRAP_S>(_texparams.wrapModes[0]);
      skyboxtex->SetWrap<WrapParam::WRAP_T>(_texparams.wrapModes[1]);
      skyboxtex->SetWrap<WrapParam::WRAP_R>(_texparams.wrapModes[2]);

      skyboxtex->SetFilter<FilterParam::MIN_FILTER>(_texparams.filterModes[0]);
      skyboxtex->SetFilter<FilterParam::MAG_FILTER>(_texparams.filterModes[1]);

      if (_currentTexOption == 3) {
        TransformPtr cubetrans = _cube->GetTransform();
        cubetrans->Rotate(0.004f, glm::vec3(0.0f, 1.0f, 0.0f));

        MaterialPtr cubemat = _cube->GetMeshRenderer()->GetMaterial();
        mvp = _mvpMat * cubetrans->GetLocalToWorldMatrix();
        cubemat->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
        cubemat->SetSimpleUniformValue<float32, 1>("size", 5.0f);

        TransformPtr spheretrans = _sphere->GetTransform();
        spheretrans->Rotate(0.004f, glm::vec3(0.0f, 1.0f, 0.0f));

        MaterialPtr spheremat = _sphere->GetMeshRenderer()->GetMaterial();
        mvp = _mvpMat * spheretrans->GetLocalToWorldMatrix();
        spheremat->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);

        TexturePtr spheretex = spheremat->GetMainTexture();
        spheretex->SetWrap<WrapParam::WRAP_S>(_texparams.wrapModes[0]);
        spheretex->SetWrap<WrapParam::WRAP_T>(_texparams.wrapModes[1]);
        spheretex->SetWrap<WrapParam::WRAP_R>(_texparams.wrapModes[2]);

        spheretex->SetFilter<FilterParam::MIN_FILTER>(
            _texparams.filterModes[0]);
        spheretex->SetFilter<FilterParam::MAG_FILTER>(
            _texparams.filterModes[1]);
      }
    }
  }
  // volumn quad material setting
  {
    GeometryPtr geom = nullptr;
    if (_currentTexOption == 2)
      geom = _volumnQuad;

    if (geom != nullptr) {
      // refer to opengl programing guide 8th source code
      mat4 rotmat = glm::identity<mat3>();
      glm::vec3 x_axis(1.0f, 0.0f, 0.0f);
      glm::vec3 y_axis(0.0f, 1.0f, 0.0f);
      glm::vec3 z_axis(0.0f, 0.0f, 1.0f);
      mat4 unifmat =
          glm::rotate(rotmat, _updateTime * glm::radians<float32>(10.0f),
                      x_axis) *
          glm::rotate(rotmat, _updateTime * glm::radians<float32>(10.0f),
                      y_axis) *
          glm::rotate(rotmat, _updateTime * glm::radians<float32>(10.0f),
                      z_axis);

      MaterialPtr mat = geom->GetMeshRenderer()->GetMaterial();
      mat->SetMatrixUniformValue<float32, 4, 4>("mvp", _mvpMat);
      vec4 tintColor(_tintcolor[0], _tintcolor[1], _tintcolor[1],
                     _tintcolor[3]);
      mat->SetVecUniformValue<float32, 4>("tintColor", tintColor);
      mat->SetTextureTiling("mainTex", _texTiling);
      mat->SetTextureOffset("mainTex", _texOffset);
      mat->SetMatrixUniformValue<float32, 4, 4>("rotmat", unifmat);

      // set sprite texture parameters
      TexturePtr tex = mat->GetTexture("mainTex");
      tex->SetWrap<WrapParam::WRAP_S>(_texparams.wrapModes[0]);
      tex->SetWrap<WrapParam::WRAP_T>(_texparams.wrapModes[1]);
      tex->SetWrap<WrapParam::WRAP_R>(_texparams.wrapModes[2]);

      tex->SetFilter<FilterParam::MIN_FILTER>(_texparams.filterModes[0]);
      tex->SetFilter<FilterParam::MAG_FILTER>(_texparams.filterModes[1]);

      tex->SetTexBorderColor(_texparams.bordercolorParameter,
                             _texparams.borderColor);
    }
  }
  {
    GeometryPtr geom = nullptr;
    if (_currentTexOption == 6)
      geom = _nurbsSurface;

    if (geom != nullptr) {
      mat4 mat_tex_view = _projTexCamera->GetViewMatrix();
      mat4 mat_tex_proj = _projTexCamera->GetProjectionMatrix();
      mat4 mat_tex_vp = mat_tex_proj * mat_tex_view;

      TransformPtr nurbs_trans = _nurbsSurface->GetTransform();
      // nurbs_trans->Rotate(0.004f, glm::vec3(0.0f, 1.0f, 1.0f));

      mat4 world = nurbs_trans->GetLocalToWorldMatrix();
      mat4 mvp = _mvpMat * world;

      MaterialPtr nurbs_mat = _nurbsSurface->GetMeshRenderer()->GetMaterial();
      nurbs_mat->SetMatrixUniformValue<float32, 4, 4>("texvp", mat_tex_vp);
      nurbs_mat->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);

      MaterialPtr quadMat = _projTexQuad->GetMeshRenderer()->GetMaterial();
      TransformPtr quadTrans = _projTexQuad->GetTransform();
      world = quadTrans->GetLocalToWorldMatrix();
      mvp = _mvpMat * world;
      quadMat->SetMatrixUniformValue<float32, 4, 4>("texvp", mat_tex_vp);
      quadMat->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
    }
  }

#ifdef _WIN32
  _updateTime += 0.02f;
#elif defined(__linux__) || defined(__APPLE__)
  _updateTime += 0.0002f;
#endif
}

void TextureExploreView::RenderImpl() {
  switch (_currentTexOption) {
  default:
    RenderGeometryEx(_currentTexOption);
    break;
  }
}
void TextureExploreView::OnGUI() {
  ImGui::Begin(u8"纹理小观");
  {
    if (ImGui::RadioButton(u8"一维纹理", &_currentTexOption, 0)) {
      ResetGUI();
      CreateSprite1D();
    }
    if (ImGui::RadioButton(u8"二维纹理", &_currentTexOption, 1)) {
      ResetGUI();
      CreateSprite();
    }
    if (ImGui::RadioButton(u8"三维纹理", &_currentTexOption, 2)) {
      ResetGUI();
      CreateVolumnTexture();
    }
    if (ImGui::RadioButton(u8"立方体纹理", &_currentTexOption, 3)) {
      ResetGUI();
      CreateSkybox();

      ImageManager &imageMgr = ImageMgrInstance();
      std::string imageFilename = {"Assets/Textures/TantolundenCube.dds"};
      ImagePtr image = imageMgr.ReadImage(imageFilename.c_str());

      CreateCube(image);
      CreateIconSphere(image);
    }
    // if(ImGui::RadioButton(u8"程序纹理", &_current_tex_option, 4))
    // {
    //     ResetGUI();
    // }
    // if(ImGui::RadioButton(u8"法线纹理", &_current_tex_option, 5))
    // {
    //     ResetGUI();
    // }
    if (ImGui::RadioButton(u8"投影纹理", &_currentTexOption, 6)) {
      ResetGUI();
      CreateNURBSSurface();
    }
    // if(ImGui::RadioButton(u8"纹理视图", &_current_tex_option, 7))
    // {
    //     ResetGUI();
    // }
  }
  ImGui::End();

  ImGui::Begin(u8"纹理设置");
  {
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    _texparamsTabitem[0] = true;
    _texparamsTabitem[1] = true;
    _texparamsTabitem[2] = true;
    _texparamsTabitem[3] = true;
    ImGui::BeginTabBar(u8"纹理参数", ImGuiTabBarFlags_None);

    if (ImGui::BeginTabItem(u8"纹理环绕", &(_texparamsTabitem[0]))) {
      bool wrap_option_clicked = false;
      ImGui::BeginGroup();
      ImGui::Text(u8"参数:");
      ImGui::Indent();
      wrap_option_clicked = ImGui::RadioButton(u8"Wrap_S", &_wrapOption, 0);
      wrap_option_clicked |= ImGui::RadioButton(u8"Wrap_T", &_wrapOption, 1);
      wrap_option_clicked |= ImGui::RadioButton(u8"Wrap_R", &_wrapOption, 2);
      ImGui::EndGroup();

      ImGui::SameLine();
      OnWrapModeGUI(_wrapModes[_wrapOption]);

      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem(u8"纹理过滤", &(_texparamsTabitem[1]))) {
      bool filter_option_clicked = false;
      ImGui::BeginGroup();
      ImGui::Text(u8"参数:");
      ImGui::Indent();
      filter_option_clicked =
          ImGui::RadioButton(u8"Minification", &_filterOption, 0);
      filter_option_clicked |=
          ImGui::RadioButton(u8"Magnification", &_filterOption, 1);
      ImGui::EndGroup();

      ImGui::SameLine();
      OnFilterModeGUI(_filterModes[_filterOption]);

      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem(u8"纹理抖动", &(_texparamsTabitem[2]))) {
      bool swizzle_option_clicked = false;
      ImGui::BeginGroup();
      ImGui::Text(u8"参数:");
      ImGui::Indent();
      swizzle_option_clicked =
          ImGui::RadioButton(u8"Swizzle_R", &_swizzleOption, 0);
      swizzle_option_clicked |=
          ImGui::RadioButton(u8"Swizzle_G", &_swizzleOption, 1);
      swizzle_option_clicked |=
          ImGui::RadioButton(u8"Swizzle_B", &_swizzleOption, 2);
      swizzle_option_clicked |=
          ImGui::RadioButton(u8"Swizzle_A", &_swizzleOption, 3);
      swizzle_option_clicked |=
          ImGui::RadioButton(u8"Swizzle_RGBA", &_swizzleOption, 4);
      ImGui::EndGroup();

      ImGui::SameLine();
      OnSwizzleModeGUI();

      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("其他", &(_texparamsTabitem[3]))) {
      ImGui::Checkbox(u8"Lod Bias", &_enableLodBias);
      if (_enableLodBias) {
        ImGui::SameLine();
        ImGui::SliderFloat("Bias Value", &_lodbiasValue, -10.0f, 10.0f);
      }

      ImGui::Checkbox(u8"Border Color", &_enableBorderColor);
      if (_enableBorderColor) {
        ImGui::SameLine();
        ImGui::ColorEdit4("Color", glm::value_ptr<float32>(_bordercolor));
      }

      ImGui::Checkbox(u8"显示天空盒", &_enableSkybox);
      if (_enableSkybox) {
        if (_skybox == nullptr) {
          CreateSkybox();
        }
      }

      ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
  }
  ImGui::End();

  ImGui::Begin(u8"其他...");
  {
    ImGui::DragFloat2("Tiling", glm::value_ptr(_texTiling), 0.1f, 0.1f, 10.0f);
    ImGui::DragFloat2("Offset", glm::value_ptr(_texOffset), 0.01f, -1.0f, 1.0f);
    ImGui::ColorEdit4("Tint Color", _tintcolor);
  }
  ImGui::End();

  // ImGui::FileBrowser fileDialog(ImGuiFileBrowserFlags_SelectDirectory);

  // // open file dialog when user clicks this button
  // if (ImGui::Button("open file dialog"))
  //     fileDialog.Open();

  // fileDialog.Display();

  // if (fileDialog.HasSelected())
  // {
  //     std::cout << "Selected filename" << fileDialog.GetSelected().string()
  //     << std::endl; fileDialog.ClearSelected();
  // }

  this->SetTexparams();
}

void TextureExploreView::OnWrapModeGUI(int32 &wrap_mode_option) {
  bool wrap_mode_clicked = false;
  ImGui::BeginGroup();
  ImGui::Text(u8"模式:");
  ImGui::Indent();
  wrap_mode_clicked = ImGui::RadioButton(u8"REPEAT", &wrap_mode_option, 0);
  wrap_mode_clicked |= ImGui::RadioButton(u8"CLAMP", &wrap_mode_option, 1);
  wrap_mode_clicked |=
      ImGui::RadioButton(u8"CLAMP_TO_EDGE", &wrap_mode_option, 2);
  wrap_mode_clicked |=
      ImGui::RadioButton(u8"CLAMP_TO_BORDER", &wrap_mode_option, 3);
  ImGui::EndGroup();
}

void TextureExploreView::OnFilterModeGUI(int32 &filter_mode_option) {
  bool filter_mode_clicked = false;
  ImGui::BeginGroup();
  ImGui::Text(u8"模式:");
  ImGui::Indent();
  filter_mode_clicked = ImGui::RadioButton(u8"NEAREST", &filter_mode_option, 0);
  filter_mode_clicked |= ImGui::RadioButton(u8"LINEAR", &filter_mode_option, 1);
  filter_mode_clicked |=
      ImGui::RadioButton(u8"NEAREST_MIPMAP_NEAREST", &filter_mode_option, 2);
  filter_mode_clicked |=
      ImGui::RadioButton(u8"NEAREST_MIPMAP_LINEAR", &filter_mode_option, 3);
  filter_mode_clicked |=
      ImGui::RadioButton(u8"LINEAR_MIPMAP_NEAREST", &filter_mode_option, 4);
  filter_mode_clicked |=
      ImGui::RadioButton(u8"LINEAR_MIPMAP_LINEAR", &filter_mode_option, 5);
  ImGui::EndGroup();
}

void TextureExploreView::OnSwizzleModeGUI() {
  ImGui::BeginGroup();
  ImGui::Text(u8"模式:");
  ImGui::Indent();

  char const *items[6] = {"RED", "GREEN", "BLUE", "ALPHA", "ZERO", "ONE"};
  if (_swizzleOption == 4 || _swizzleOption == 0)
    ImGui::Combo(u8"Swizzle RED", &(_swizzleMasks[0]), items, 6);

  if (_swizzleOption == 4 || _swizzleOption == 1)
    ImGui::Combo(u8"Swizzle GREEN", &(_swizzleMasks[1]), items, 6);

  if (_swizzleOption == 4 || _swizzleOption == 2)
    ImGui::Combo(u8"Swizzle BLUE", &(_swizzleMasks[2]), items, 6);

  if (_swizzleOption == 4 || _swizzleOption == 3)
    ImGui::Combo(u8"Swizzle ALPHA", &(_swizzleMasks[3]), items, 6);

  ImGui::EndGroup();
}

void TextureExploreView::ResetGUI() {
  _wrapOption = -1;
  _filterOption = -1;
  _swizzleOption = -1;
  _enableLodBias = false;
  _enableBorderColor = false;
  _wrapModes[0] = _wrapModes[1] = _wrapModes[2] = -1;
  _filterModes[0] = _filterModes[1] = -1;
  _swizzleMasks[0] = 0;
  _swizzleMasks[1] = 1;
  _swizzleMasks[2] = 2;
  _swizzleMasks[3] = 3;

  _tintcolor[0] = _tintcolor[1] = _tintcolor[2] = _tintcolor[3] = 1.0f;
  _bordercolor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

  _texTiling = vec2(1.0f, 1.0f);
  _texOffset = vec2(0.0f, 0.0f);
}

void TextureExploreView::SetTexparams() {
  // set wrap mode
  for (int32 i = 0; i < 3; i++) {
    WrapMode mode = GetWrapMode(_wrapModes[i]);
    _texparams.wrapModes[i] = mode;
  }

  // set filter
  for (int32 i = 0; i < 2; i++) {
    FilterMode mode = GetFilterMode(_filterModes[i]);
    _texparams.filterModes[i] = mode;
  }

  // set swizzle
  _texparams.swizzleParameter = GetSwizzleParam(_swizzleOption);
  SwizzleMask masks[4];
  for (int32 i = 0; i < 4; i++) {
    _texparams.swizzle[i] = GetSwizzleMask(_swizzleMasks[i]);
  }

  // set lod bias
  _texparams.lodParameter =
      _enableLodBias ? LodBiasParam::LOD_BIAS : LodBiasParam::NONE;
  _texparams.lodbias = _lodbiasValue;

  _texparams.bordercolorParameter = _enableBorderColor
                                        ? TextureBorderColorParam::BORDER_COLOR
                                        : TextureBorderColorParam::NONE;
  _texparams.borderColor = _bordercolor;
}

WrapMode TextureExploreView::GetWrapMode(int32 wrap_mode_option) {
  if (wrap_mode_option == -1)
    return WrapMode::NONE;

  WrapMode modes[5] = {WrapMode::REPEAT, WrapMode::CLAMP,
                       WrapMode::CLAMP_TO_EDGE, WrapMode::CLAMP_TO_BORDER,
                       WrapMode::NONE};

  return modes[wrap_mode_option];
}

FilterMode TextureExploreView::GetFilterMode(int32 filter_mode_option) {
  if (filter_mode_option == -1) {
    return FilterMode::NONE;
  }

  // FilterMode modes[7] = { FilterMode::NEAREST, FilterMode::LINEAR,
  // FilterMode::NEAREST_MIPMAP_NEAREST,
  //     FilterMode::NEAREST_MIPMAP_LINEAR, FilterMode::LINEAR_MIPMAP_NEAREST,
  //     FilterMode::LINEAR_MIPMAP_LINEAR, FilterMode::NONE };

  return FilterMode((int)FilterMode::NEAREST +
                    filter_mode_option); // modes[filter_mode_option];
}

SwizzleParam TextureExploreView::GetSwizzleParam(int32 swizzle_option) {
  if (swizzle_option == -1) {
    return SwizzleParam::NONE;
  }

  // SwizzleParam params[5] = { SwizzleParam::SWIZZLE_R,
  // SwizzleParam::SWIZZLE_G, SwizzleParam::SWIZZLE_B,
  //     SwizzleParam::SWIZZLE_A, SwizzleParam::SWIZZLE_RGBA };

  return SwizzleParam((int)SwizzleParam::SWIZZLE_R +
                      swizzle_option); // params[swizzle_option];
}

SwizzleMask TextureExploreView::GetSwizzleMask(int32 swizzle_mask_option) {
  SwizzleMask masks[6] = {SwizzleMask::RED,  SwizzleMask::GREEN,
                          SwizzleMask::BLUE, SwizzleMask::ALPHA,
                          SwizzleMask::ZERO, SwizzleMask::ONE};

  return masks[swizzle_mask_option];
}

void TextureExploreView::CreateSprite() {
  if (_sprite != nullptr)
    return;

  ImageManager &imageMgr = ImageMgrInstance();
  std::string imageFilename = {"Assets/Textures/test3.png"};
  ImagePtr image = imageMgr.ReadImage(imageFilename.c_str());

  Texture2DPtr texture = nullptr;
  if (image != nullptr) {
    texture = std::make_shared<Texture2D>(true);
    texture->CreateFromImage(image);

    // SamplerPtr sampler = std::make_shared<Sampler>();
    // texture->SetSampler(sampler);
  }

  // initialise sprite
  if (texture != nullptr) {
    // glm::vec2 size = glm::vec2(texture->GetWidth(0), texture->GetHeight(0)) /
    // (float32)100.0f * 2.0f;
    _sprite = std::make_shared<Sprite>(texture);
    // _sprite = std::make_shared<Sprite>(texture, size);
    CreateGeometry(_sprite, 1);
  }
}

void TextureExploreView::CreateSprite1D() {
  if (_sprite1D == nullptr) {
    ImageData data;
    data.target = GL_TEXTURE_1D;
    data.internalFormat = GL_RGBA8;
    data.format = GL_RGBA;
    data.type = GL_UNSIGNED_BYTE;
    data.mipLevels = 1;
    data.totalDataSize = 3 * 1 * 4;

    data.mip[0].width = 3;
    data.mip[0].height = 1;
    data.mip[0].mipStride = data.totalDataSize;

    GLubyte *bytes =
        new GLubyte[12]{255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255};
    data.mip[0].data = bytes;

    ImagePtr image = std::make_shared<Image>();
    image->SetImageSource(std::move(data));

    Texture1DPtr texture1D = std::make_shared<Texture1D>(true);
    texture1D->CreateFromImage(image);

    _sprite1D = std::make_shared<Sprite>(texture1D, glm::vec2(5, 5));

    CreateGeometry(_sprite1D, 0);
  }
}

void TextureExploreView::CreateVolumnTexture() {
  if (_volumnQuad == nullptr) {
    vec2 size(10.0f, 10.0f);
    _volumnQuad = std::make_shared<Quad>(size, MeshDataFlag(8));
    // _volumnQuad->GenerateMeshInternal();

    MeshRendererPtr renderer = std::make_shared<MeshRenderer>();
    MaterialPtr mat = std::make_shared<VolumnQuadMaterial>();
    renderer->SetMaterial(mat);
    mat = renderer->GetMaterial();

    ImageManager &imageMgr = ImageMgrInstance();
    std::string imageFilename = {"Assets/Textures/cloud.dds"};
    ImagePtr image = imageMgr.ReadImage(imageFilename.c_str());

    Texture3DPtr volumntex = std::make_shared<Texture3D>(true);
    volumntex->CreateFromImage(image);
    mat->SetMainTexture(volumntex);

    renderer->SetMesh(_volumnQuad->GetMesh());
    _volumnQuad->SetMeshRenderer(renderer);

    CreateGeometry(_volumnQuad, 2);
  }
}

void TextureExploreView::CreateSkybox() {
  if (_skybox == nullptr) {
    _skybox = std::make_shared<Cube>(2.0f, MeshDataFlag::DEFAULT);
    // _skybox->GenerateMeshInternal();

    MeshRendererPtr renderer = std::make_shared<MeshRenderer>();
    MaterialPtr mat = std::make_shared<SkyboxMaterial>();

    TextureCubePtr cubemap = std::make_shared<TextureCube>(true);
    ImageManager &imageMgr = ImageMgrInstance();
    // ImageReadInfo imageFilename = {"Assets/Textures/TantolundenCube.dds"};
    // ImagePtr image = imageMgr.ReadImage(imageFilename);

    // ImageReadInfo front_info =
    // {"Assets/Textures/plains-of-abraham/plains-of-abraham_ft.tga"}; ImagePtr
    // front_image = imageMgr.ReadImage(front_info); ImageReadInfo back_info =
    // {"Assets/Textures/plains-of-abraham/plains-of-abraham_bk.tga"}; ImagePtr
    // back_image = imageMgr.ReadImage(back_info); ImageReadInfo left_info =
    // {"Assets/Textures/plains-of-abraham/plains-of-abraham_lf.tga"}; ImagePtr
    // left_image = imageMgr.ReadImage(left_info); ImageReadInfo right_info =
    // {"Assets/Textures/plains-of-abraham/plains-of-abraham_rt.tga"}; ImagePtr
    // right_image = imageMgr.ReadImage(right_info); ImageReadInfo top_info =
    // {"Assets/Textures/plains-of-abraham/plains-of-abraham_up.tga"}; ImagePtr
    // top_image = imageMgr.ReadImage(top_info); ImageReadInfo down_info =
    // {"Assets/Textures/plains-of-abraham/plains-of-abraham_dn.tga"}; ImagePtr
    // down_image = imageMgr.ReadImage(down_info);

    std::string front_info = {"Assets/Textures/skybox/front.png"};
    ImagePtr front_image = imageMgr.ReadImage(front_info.c_str());
    std::string back_info = {"Assets/Textures/skybox/back.png"};
    ImagePtr back_image = imageMgr.ReadImage(back_info.c_str());
    std::string left_info = {"Assets/Textures/skybox/left.png"};
    ImagePtr left_image = imageMgr.ReadImage(left_info.c_str());
    std::string right_info = {"Assets/Textures/skybox/right.png"};
    ImagePtr right_image = imageMgr.ReadImage(right_info.c_str());
    std::string top_info = {"Assets/Textures/skybox/top.png"};
    ImagePtr top_image = imageMgr.ReadImage(top_info.c_str());
    std::string down_info = {"Assets/Textures/skybox/bottom.png"};
    ImagePtr down_image = imageMgr.ReadImage(down_info.c_str());

    // ImageReadInfo front_info = {"Assets/Textures/sor_sea/sea_ft.png"};
    // ImagePtr front_image = imageMgr.ReadImage(front_info);
    // ImageReadInfo back_info = {"Assets/Textures/sor_sea/sea_bk.png"};
    // ImagePtr back_image = imageMgr.ReadImage(back_info);
    // ImageReadInfo left_info = {"Assets/Textures/sor_sea/sea_lf.png"};
    // ImagePtr left_image = imageMgr.ReadImage(left_info);
    // ImageReadInfo right_info = {"Assets/Textures/sor_sea/sea_rt.png"};
    // ImagePtr right_image = imageMgr.ReadImage(right_info);
    // ImageReadInfo top_info = {"Assets/Textures/sor_sea/sea_up.png"};
    // ImagePtr top_image = imageMgr.ReadImage(top_info);
    // ImageReadInfo down_info = {"Assets/Textures/sor_sea/sea_dn.png"};
    // ImagePtr down_image = imageMgr.ReadImage(down_info);

    // cubemap->CreateFromImage(image);

    cubemap->SetPositiveX(right_image);
    cubemap->SetPositiveY(top_image);
    cubemap->SetPositiveZ(front_image);
    cubemap->SetNegativeX(left_image);
    cubemap->SetNegativeY(down_image);
    cubemap->SetNegativeZ(back_image);
    cubemap->InitStorageByOthers();

    renderer->SetMaterial(mat);
    mat = renderer->GetMaterial();

    mat->SetMainTexture(cubemap);
    renderer->SetMesh(_skybox->GetMesh());

    _skybox->SetMeshRenderer(renderer);

    CreateGeometry(_skybox, 3);

    // CreateCube(image);
    // CreateIconSphere(image);
  }
}

void TextureExploreView::CreateCube(ImagePtr image) {
  if (_cube == nullptr) {
    _cube = std::make_shared<Cube>(5.0f, MeshDataFlag::DEFAULT);
    // _cube->GenerateMeshInternal();

    TransformPtr trans = _cube->GetTransform();
    trans->Translate(glm::vec3(0.0f, 0.0f, -15.0f));
    _camera->AddChild(_cube);

    MeshRendererPtr renderer = std::make_shared<MeshRenderer>();
    MaterialPtr mat = std::make_shared<CubeMaterial>();

    TextureCubePtr cubemap = std::make_shared<TextureCube>(true);
    cubemap->CreateFromImage(image);

    renderer->SetMaterial(mat);
    mat = renderer->GetMaterial();
    mat->SetMainTexture(cubemap);

    renderer->SetMesh(_cube->GetMesh());
    _cube->SetMeshRenderer(renderer);

    CreateGeometry(_cube, 15);
  }
}
void TextureExploreView::CreateIconSphere(ImagePtr image) {
  if (_sphere == nullptr) {
    _sphere =
        std::make_shared<IcosahedronSphere>(2.5f, 50, MeshDataFlag::DEFAULT);
    // _sphere->GenerateMeshInternal();

    TransformPtr trans = _sphere->GetTransform();
    trans->Translate(glm::vec3(-3.5f, 0.0f, 0.0f));

    MeshRendererPtr renderer = std::make_shared<MeshRenderer>();
    MaterialPtr mat = std::make_shared<SphereMaterial>();

    TextureCubePtr cubemap = std::make_shared<TextureCube>(true);
    cubemap->CreateFromImage(image);

    renderer->SetMaterial(mat);
    mat->SetMainTexture(cubemap);
    mat = renderer->GetMaterial();

    renderer->SetMesh(_sphere->GetMesh());
    _sphere->SetMeshRenderer(renderer);

    CreateGeometry(_sphere, 14);
  }
}

void TextureExploreView::CreateNURBSSurface() {
  if (_nurbsSurface == nullptr) {
    glm::vec4 *control_points = new glm::vec4[25];

    int32 n = 0;
    control_points[0] = glm::vec4(-10.f, 0.5f, 0.0f, 1.0f);
    control_points[1] = glm::vec4(-8.5f, 3.5f, -3.0f, 1.0f);
    control_points[2] = glm::vec4(-8.f, 4.5f, -6.0f, 1.0f);
    control_points[3] = glm::vec4(-10.5f, 0.5f, -9.0f, 1.0f);
    control_points[4] = glm::vec4(-8.5f, -1.5f, -12.0f, 1.0f);

    // _nurbs_control_line = Mesh::CreateLineMeshEx(control_points, 5);
    // CreateGeometry(_nurbs_control_line->GetSubMesh(0), 19);

    n = 5;
    control_points[0 + n] = glm::vec4(-10.f + n * 1.0f, 0.0f, 0.0f, 1.0f);
    control_points[1 + n] = glm::vec4(-8.5f + n * 1.0f, 5.5f, -3.0f, 1.0f);
    control_points[2 + n] = glm::vec4(-8.f + n * 1.0f, 6.5f, -6.0f, 1.0f);
    control_points[3 + n] = glm::vec4(-10.5f + n * 1.0f, 0.5f, -9.0f, 1.0f);
    control_points[4 + n] = glm::vec4(-8.5f + n * 1.0f, 3.5f, -12.0f, 1.0f);

    n = 10;
    control_points[0 + n] = glm::vec4(-10.f + n * 1.0f, 0.0f, 0.0f, 1.0f);
    control_points[1 + n] = glm::vec4(-8.5f + n * 1.0f, 5.5f, -3.0f, 1.0f);
    control_points[2 + n] = glm::vec4(-8.f + n * 1.0f, 6.5f, -6.0f, 1.0f);
    control_points[3 + n] = glm::vec4(-10.5f + n * 1.0f, 0.5f, -9.0f, 1.0f);
    control_points[4 + n] = glm::vec4(-8.5f + n * 1.0f, 3.5f, -12.0f, 1.0f);

    n = 15;
    control_points[0 + n] = glm::vec4(-10.f + n * 1.0f, 0.0f, 0.0f, 1.0f);
    control_points[1 + n] = glm::vec4(-8.5f + n * 1.0f, 5.5f, -3.0f, 1.0f);
    control_points[2 + n] = glm::vec4(-8.f + n * 1.0f, 6.5f, -6.0f, 1.0f);
    control_points[3 + n] = glm::vec4(-10.5f + n * 1.0f, 0.5f, -9.0f, 1.0f);
    control_points[4 + n] = glm::vec4(-8.5f + n * 1.0f, 3.5f, -12.0f, 1.0f);

    n = 20;
    control_points[0 + n] = glm::vec4(-10.f + n * 1.0f, 0.0f, 0.0f, 1.0f);
    control_points[1 + n] = glm::vec4(-8.5f + n * 1.0f, 5.5f, -3.0f, 1.0f);
    control_points[2 + n] = glm::vec4(-8.f + n * 1.0f, 6.5f, -6.0f, 1.0f);
    control_points[3 + n] = glm::vec4(-10.5f + n * 1.0f, 0.5f, -9.0f, 1.0f);
    control_points[4 + n] = glm::vec4(-8.5f + n * 1.0f, 3.5f, -12.0f, 1.0f);

    Knot *u_knots = new Knot[9];
    u_knots[0].u = 0.0f;
    u_knots[8].u = 1.0f;
    u_knots[1].u = 0.0f;
    u_knots[2].u = 0.0f;
    u_knots[3].u = 0.0f;
    u_knots[4].u = 0.5f;
    u_knots[5].u = 1.f;
    u_knots[6].u = 1.f;
    u_knots[7].u = 1.f;

    u_knots[3].multiplity = 3;
    u_knots[8].multiplity = 3;

    Knot *v_knots = new Knot[9];
    v_knots[0].u = 0.0f;
    v_knots[8].u = 1.0f;
    v_knots[1].u = 0.0f;
    v_knots[2].u = 0.0f;
    v_knots[3].u = 0.0f;
    v_knots[4].u = 0.5f;
    v_knots[5].u = 1.f;
    v_knots[6].u = 1.f;
    v_knots[7].u = 1.f;

    v_knots[3].multiplity = 3;
    v_knots[8].multiplity = 3;

    _nurbsSurface = std::make_shared<NURBSSurface>(5, 3, 5, 3, control_points,
                                                   u_knots, v_knots);
    // _nurbsSurface->SetControlPoints(control_points, 25);
    // _nurbsSurface->SetUKnots(u_knots, 9);
    // _nurbsSurface->SetVKnots(v_knots, 9);
    // _nurbsSurface->GenerateMeshInternal();

    TransformPtr trans = _nurbsSurface->GetTransform();
    trans->Translate(glm::vec3(-6.0f, 0.0f, 0.0f));

    MeshPtr mesh = _nurbsSurface->GetMesh();
    SubMeshPtr submesh = mesh->GetSubMesh(0);

    MeshRendererPtr renderer = std::make_shared<MeshRenderer>();
    MaterialPtr mat = std::make_shared<ProjectionMappingMaterial>();

    ImageManager &imageMgr = ImageMgrInstance();
    std::string imageFilename = {"Assets/Textures/test3.png"};
    ImagePtr image = imageMgr.ReadImage(imageFilename.c_str());

    Texture2DPtr texture = nullptr;
    texture = std::make_shared<Texture2D>(true);
    texture->CreateFromImage(image);

    texture->SetWrap<WrapParam::WRAP_S>(WrapMode::CLAMP_TO_BORDER);
    texture->SetWrap<WrapParam::WRAP_T>(WrapMode::CLAMP_TO_BORDER);

    texture->SetTexBorderColor(TextureBorderColorParam::BORDER_COLOR,
                               vec4(1.0f, 1.0f, 1.0f, 1.0f));

    renderer->SetMaterial(mat);
    mat = renderer->GetMaterial();

    mat->SetMainTexture(texture);
    renderer->SetMesh(_nurbsSurface->GetMesh());

    _nurbsSurface->SetMeshRenderer(renderer);

    vec2 size(10.0f, 10.0f);
    _projTexQuad = std::make_shared<Quad>(size, MeshDataFlag(8));
    // _projTexQuad->GenerateMeshInternal();

    TransformPtr quadTrans = _projTexQuad->GetTransform();
    quadTrans->Translate(glm::vec3(6.0f, 0.0f, 0.0f));

    MeshRendererPtr quadRenderer = std::make_shared<MeshRenderer>();
    MaterialPtr quadMat = std::make_shared<ProjectionMappingMaterial>();

    quadRenderer->SetMaterial(quadMat);
    quadMat = quadRenderer->GetMaterial();
    quadMat->SetMainTexture(texture);

    quadRenderer->SetMesh(_projTexQuad->GetMesh());
    _projTexQuad->SetMeshRenderer(quadRenderer);

    CreateGeometry(_nurbsSurface, 6);
    CreateGeometry(_projTexQuad, 13);

    SAFE_DEL_ARR(control_points);
    SAFE_DEL_ARR(u_knots);
    SAFE_DEL_ARR(v_knots);
  }
}

void TextureExploreView::RenderGeometryEx(int index) {
  if (index != 3 && _enableSkybox) {
    RenderGeometry(_skybox, 3);
  }

  GeometryPtr geom = nullptr;
  switch (index) {
  case 0:
    geom = _sprite1D;
    break;
  case 1:
    geom = _sprite;
    break;
  case 2:
    geom = _volumnQuad;
    break;
  case 3:
    geom = _skybox;
    if (_cube != nullptr)
      RenderGeometry(_cube, 15);
    if (_sphere != nullptr)
      RenderGeometry(_sphere, 14);
    break;
  case 6:
    geom = _nurbsSurface;
    RenderGeometry(_projTexQuad, 13);
  default:
    break;
  }

  if (geom != nullptr) {
    RenderGeometry(geom, index);
  }
}

void TextureExploreView::CreateGeometry(GeometryPtr geom, uint32 index) {
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
  glBindVertexBuffer(0, _vbos[index], 0, sizeof(vec3));
  glEnableVertexAttribArray(0);
  glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexAttribBinding(0, 0);

  // bind vertex uv
  if ((geom->GetMeshDataFlag() & MeshDataFlag::HAS_UV) != 0) {
    glBindVertexBuffer(1, _vbos[index], verticenum * 12, sizeof(vec4));
    glEnableVertexAttribArray(4);
    glVertexAttribFormat(4, 4, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(4, 1);
  }

  glBindVertexArray(0);
}

void TextureExploreView::RenderGeometry(GeometryPtr geom, int32 index,
                                        GLenum front_face) {
  MaterialPtr mat = geom->GetMeshRenderer()->GetMaterial();
  RenderPassPtr pass = mat->GetRenderPass(0);
  if (pass == nullptr) {
    return;
  }

  ShaderProgramPtr shader = pass->GetShaderProgram();

  for (auto tex_slot : pass->GetTextureSlots()) {
    tex_slot.second.Apply();
  }

  ShaderProgramUse use(shader);
  for (auto loc : pass->GetUniformLocations()) {
    loc.second.Bind();
  }

  glEnable(GL_DEPTH_TEST);
  if (index == 3)
    glDepthFunc(GL_LEQUAL);
  else
    glDepthFunc(GL_LESS);

  if (index == 1 || index == 2) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  glFrontFace(front_face);
  glDisable(GL_CULL_FACE);

  SubMeshPtr submesh = geom->GetMesh()->GetSubMesh(0);

  // draw command use vertex array object
  glBindVertexArray(_vaos[index]);
  glDrawElements(GL_TRIANGLES, submesh->GetIndiceNum(), GL_UNSIGNED_INT, NULL);

  glFrontFace(GL_CCW);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glDisable(GL_BLEND);

  for (auto tex_slot : pass->GetTextureSlots()) {
    tex_slot.second.UnBind();
  }
}

} // namespace TwinkleGraphics
