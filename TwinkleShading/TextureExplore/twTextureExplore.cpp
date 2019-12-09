

#include "imgui.h"
// #include "imgui_filebrowser.h"
#include "twImage.h"
#include "twMaterialInstance.h"

#include "twTextureExplore.h"


namespace TwinkleGraphics
{
TextureExplore::TextureExplore(std::string& name)
    : GLPlugin(name)
    , _view(nullptr)
{}

TextureExplore::~TextureExplore()
{
    SAFE_DEL(_view);
}

void TextureExplore::Install()
{
    Plugin::Install();

    // Initilize view
    Viewport viewport(Rect(0, 0, 800, 640), 17664U, RGBA(0.0f, 0.f, 0.f, 1.f));
    Camera::Ptr camera = std::make_shared<Camera>(viewport, 45.0f, 0.1f, 1000.0f);
    _view = new TextureExploreView();
    _view->SetViewCamera(camera);
    OrbitControl* camera_control = new OrbitControl(camera);
    camera_control->SetMinDistance(1.0f);
    _view->SetCameraControl(camera_control);
    _view->Initialize();

    _views[_views_count++] = _view;
}

void TextureExplore::UnInstall()
{
    _view->Destroy();
    SAFE_DEL(_view);

    Plugin::UnInstall();
}




void TextureExploreView::Initialize()
{
    //create vertex buffer object
    _vbos = new uint32[16];
    glGenBuffers(16, _vbos);
    _ebos = new uint32[16];
    glGenBuffers(16, _ebos);

    //create vertex array object
    _vaos = new uint32[16];
    glGenVertexArrays(16, _vaos);

    Viewport viewport(Rect(0, 0, 800, 640), 17664U, RGBA(0.0f, 0.f, 0.f, 1.f));
    _proj_tex_camera = std::make_shared<Camera>(viewport, 30.0f, 0.1f, 1000.0f);
    // _proj_tex_camera->SetOrientation(quat(1.0f, 0.0f, 0.0f, glm::pi<float32>() * 0.5f));
    _proj_tex_camera->Translate(vec3(0.0f, 0.0f, 10.0f));

}
void TextureExploreView::Destroy()
{
    ShaderProgramUse use(nullptr);

    //unbind vao/ebo
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //Delete buffers
    glDeleteVertexArrays(16, _vaos);
    glDeleteBuffers(16, _vbos);
    glDeleteBuffers(16, _ebos);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void TextureExploreView::Advance(float64 delta_time)
{
    View::Advance(delta_time);

    const Viewport& viewport = _camera->GetViewport();
    _viewport_params = glm::vec4((float32)(viewport.Width()), (float32)(viewport.Height()), viewport.AspectRatio(), 1.0f);
    _view_mat = _camera->GetViewMatrix();
    _projection_mat = _camera->GetProjectionMatrix();
    _mvp_mat = _projection_mat * _view_mat;

    // sprite setting
    {
        Sprite::Ptr sprite = nullptr;
        if (_current_tex_option == 0)
            sprite = _sprite_1d;
        else if (_current_tex_option == 1)
            sprite = _sprite;

        if (sprite != nullptr)
        {
            Material::Ptr mat = nullptr;
            mat = sprite->GetMaterial();
            mat->SetMatrixUniformValue<float32, 4, 4>("mvp", _mvp_mat);
            vec4 tintColor(_tintcolor[0], _tintcolor[1], _tintcolor[1], _tintcolor[3]);
            mat->SetVecUniformValue<float32, 4>("tint_color", tintColor);
            mat->SetTextureTiling("main_tex", _tex_tiling);
            mat->SetTextureOffset("main_tex", _tex_offset);

            // set sprite texture parameters
            Texture::Ptr tex = sprite->GetTexture();
            tex->SetWrap<WrapParam::WRAP_S>(_texparams.wrap_modes[0]);
            if(sprite == _sprite)
                tex->SetWrap<WrapParam::WRAP_T>(_texparams.wrap_modes[1]);

            tex->SetFilter<FilterParam::MIN_FILTER>(_texparams.filter_modes[0]);
            tex->SetFilter<FilterParam::MAG_FILTER>(_texparams.filter_modes[1]);

            tex->SetTexBorderColor(_texparams.bordercolor_parameter, _texparams.border_color);
        }
    }
    // skybox setting
    {
        Geometry::Ptr geom = nullptr;
        if (_current_tex_option == 3 || _enable_skybox)
        {
            geom = _skybox;
            Material::Ptr skyboxmat = geom->GetMeshRenderer()->GetMaterial();
            mat4 rotate_mat = glm::mat4_cast(_camera->GetOrientation());
            // mat4 mvp = _projection_mat * glm::mat4(glm::mat3(_view_mat));
            mat4 mvp = _projection_mat * rotate_mat;
            
            skyboxmat->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
            
            Texture::Ptr skyboxtex = skyboxmat->GetMainTexture();
            skyboxtex->SetWrap<WrapParam::WRAP_S>(_texparams.wrap_modes[0]);
            skyboxtex->SetWrap<WrapParam::WRAP_T>(_texparams.wrap_modes[1]);
            skyboxtex->SetWrap<WrapParam::WRAP_R>(_texparams.wrap_modes[2]);

            skyboxtex->SetFilter<FilterParam::MIN_FILTER>(_texparams.filter_modes[0]);
            skyboxtex->SetFilter<FilterParam::MAG_FILTER>(_texparams.filter_modes[1]);

            if(_current_tex_option == 3)
            {
                Transform::Ptr cubetrans = _cube->GetTransform();
                cubetrans->Rotate(0.004f, glm::vec3(0.0f, 1.0f, 0.0f));

                Material::Ptr cubemat = _cube->GetMeshRenderer()->GetMaterial();
                mvp = _mvp_mat * cubetrans->GetLocalToWorldMatrix();
                cubemat->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
                cubemat->SetSimpleUniformValue<float32, 1>("size", 5.0f);

                Transform::Ptr spheretrans = _sphere->GetTransform();
                spheretrans->Rotate(0.004f, glm::vec3(0.0f, 1.0f, 0.0f));

                Material::Ptr spheremat = _sphere->GetMeshRenderer()->GetMaterial();
                mvp = _mvp_mat * spheretrans->GetLocalToWorldMatrix();
                spheremat->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
                spheremat->SetSimpleUniformValue<float32, 1>("size", 2.5f);

                Texture::Ptr spheretex = spheremat->GetMainTexture();
                spheretex->SetWrap<WrapParam::WRAP_S>(_texparams.wrap_modes[0]);
                spheretex->SetWrap<WrapParam::WRAP_T>(_texparams.wrap_modes[1]);
                spheretex->SetWrap<WrapParam::WRAP_R>(_texparams.wrap_modes[2]);

                spheretex->SetFilter<FilterParam::MIN_FILTER>(_texparams.filter_modes[0]);
                spheretex->SetFilter<FilterParam::MAG_FILTER>(_texparams.filter_modes[1]);
            }
        }
    }
    // volumn quad material setting
    {
        Geometry::Ptr geom = nullptr;
        if(_current_tex_option == 2)
            geom = _volumn_quad;

        if(geom != nullptr)
        {
            //refer to opengl programing guide 8th source code
            mat4 rotmat = glm::identity<mat3>();
            glm::vec3 x_axis(1.0f, 0.0f, 0.0f);
            glm::vec3 y_axis(0.0f, 1.0f, 0.0f);
            glm::vec3 z_axis(0.0f, 0.0f, 1.0f);
            mat4 unifmat = glm::rotate(rotmat, _update_time * glm::radians<float32>(10.0f), x_axis) *
                            glm::rotate(rotmat, _update_time * glm::radians<float32>(10.0f), y_axis) *
                            glm::rotate(rotmat, _update_time * glm::radians<float32>(10.0f), z_axis);

            Material::Ptr mat = geom->GetMeshRenderer()->GetMaterial();
            mat->SetMatrixUniformValue<float32, 4, 4>("mvp", _mvp_mat);
            vec4 tintColor(_tintcolor[0], _tintcolor[1], _tintcolor[1], _tintcolor[3]);
            mat->SetVecUniformValue<float32, 4>("tint_color", tintColor);
            mat->SetTextureTiling("main_tex", _tex_tiling);
            mat->SetTextureOffset("main_tex", _tex_offset);
            mat->SetMatrixUniformValue<float32, 4, 4>("rotmat", unifmat);

            // set sprite texture parameters
            Texture::Ptr tex = mat->GetTexture("main_tex");
            tex->SetWrap<WrapParam::WRAP_S>(_texparams.wrap_modes[0]);
            tex->SetWrap<WrapParam::WRAP_T>(_texparams.wrap_modes[1]);
            tex->SetWrap<WrapParam::WRAP_R>(_texparams.wrap_modes[2]);

            tex->SetFilter<FilterParam::MIN_FILTER>(_texparams.filter_modes[0]);
            tex->SetFilter<FilterParam::MAG_FILTER>(_texparams.filter_modes[1]);

            tex->SetTexBorderColor(_texparams.bordercolor_parameter, _texparams.border_color);            
        }
    }
    {
        Geometry::Ptr geom = nullptr;
        if(_current_tex_option == 6)
            geom = _nurbs_surface;

        if(geom != nullptr)
        {
            mat4 mat_tex_view = _proj_tex_camera->GetViewMatrix();
            mat4 mat_tex_proj = _proj_tex_camera->GetProjectionMatrix();
            mat4 mat_tex_vp = mat_tex_proj * mat_tex_view;

            Transform::Ptr nurbs_trans = _nurbs_surface->GetTransform();
            // nurbs_trans->Rotate(0.004f, glm::vec3(0.0f, 1.0f, 1.0f));

            mat4 world = nurbs_trans->GetLocalToWorldMatrix();
            mat4 mvp = _mvp_mat * world;

            Material::Ptr nurbs_mat = _nurbs_surface->GetMeshRenderer()->GetMaterial();
            nurbs_mat->SetMatrixUniformValue<float32, 4, 4>("texvp", mat_tex_vp);
            nurbs_mat->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);

            Material::Ptr quad_mat = _proj_tex_quad->GetMeshRenderer()->GetMaterial();
            Transform::Ptr quad_trans = _proj_tex_quad->GetTransform();
            world = quad_trans->GetLocalToWorldMatrix();
            mvp = _mvp_mat * world;
            quad_mat->SetMatrixUniformValue<float32, 4, 4>("texvp", mat_tex_vp);
            quad_mat->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
        }
    }

#ifdef _WIN32 || _WIN64
    _update_time += 0.02f;
#elif defined __linux__
    _update_time += 0.0002f;
#endif
}

void TextureExploreView::RenderImpl()
{
    switch (_current_tex_option)
    {
    default:
        RenderGeometryEx(_current_tex_option);
        break;
    }

}
void TextureExploreView::OnGUI()
{
    ImGui::Begin(u8"纹理小观");
    {
        if(ImGui::RadioButton(u8"一维纹理", &_current_tex_option, 0))
        {
            ResetGUI();
            CreateSprite1D();
        }
        if(ImGui::RadioButton(u8"二维纹理", &_current_tex_option, 1))
        {
            ResetGUI();
            CreateSprite();
        }
        if(ImGui::RadioButton(u8"三维纹理", &_current_tex_option, 2))
        {
            ResetGUI();
            CreateVolumnTexture();
        }
        if(ImGui::RadioButton(u8"立方体纹理", &_current_tex_option, 3))
        {
            ResetGUI();
            CreateSkybox();

            ImageManagerInst imageMgr;
            ImageReadInfo images_info = {"Assets/Textures/TantolundenCube.dds"};
            Image::Ptr image = imageMgr->ReadImage(images_info);

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
        if(ImGui::RadioButton(u8"投影纹理", &_current_tex_option, 6))
        {
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
        _texparams_tabitem[0] = true;
        _texparams_tabitem[1] = true;
        _texparams_tabitem[2] = true;
        _texparams_tabitem[3] = true;
        ImGui::BeginTabBar(u8"纹理参数", ImGuiTabBarFlags_None);

        if (ImGui::BeginTabItem(u8"纹理环绕", &(_texparams_tabitem[0])))
        {
            bool wrap_option_clicked = false;
            ImGui::BeginGroup();
            ImGui::Text(u8"参数:");
            ImGui::Indent();
            wrap_option_clicked = ImGui::RadioButton(u8"Wrap_S", &_wrap_option, 0);
            wrap_option_clicked |= ImGui::RadioButton(u8"Wrap_T", &_wrap_option, 1);
            wrap_option_clicked |= ImGui::RadioButton(u8"Wrap_R", &_wrap_option, 2);
            ImGui::EndGroup();

            ImGui::SameLine();
            OnWrapModeGUI(_wrap_modes[_wrap_option]);

            ImGui::EndTabItem();
        }

        if(ImGui::BeginTabItem(u8"纹理过滤", &(_texparams_tabitem[1])))
        {
            bool filter_option_clicked = false;
            ImGui::BeginGroup();
            ImGui::Text(u8"参数:");
            ImGui::Indent();
            filter_option_clicked = ImGui::RadioButton(u8"Minification", &_filter_option, 0);
            filter_option_clicked |= ImGui::RadioButton(u8"Magnification", &_filter_option, 1);
            ImGui::EndGroup();

            ImGui::SameLine();
            OnFilterModeGUI(_filter_modes[_filter_option]);

            ImGui::EndTabItem();
        }

        if(ImGui::BeginTabItem(u8"纹理抖动", &(_texparams_tabitem[2])))
        {
            bool swizzle_option_clicked = false;
            ImGui::BeginGroup();
            ImGui::Text(u8"参数:");
            ImGui::Indent();
            swizzle_option_clicked = ImGui::RadioButton(u8"Swizzle_R", &_swizzle_option, 0);
            swizzle_option_clicked |= ImGui::RadioButton(u8"Swizzle_G", &_swizzle_option, 1);
            swizzle_option_clicked |= ImGui::RadioButton(u8"Swizzle_B", &_swizzle_option, 2);
            swizzle_option_clicked |= ImGui::RadioButton(u8"Swizzle_A", &_swizzle_option, 3);
            swizzle_option_clicked |= ImGui::RadioButton(u8"Swizzle_RGBA", &_swizzle_option, 4);
            ImGui::EndGroup();

            ImGui::SameLine();
            OnSwizzleModeGUI();

            ImGui::EndTabItem();
        }

        if(ImGui::BeginTabItem("其他", &(_texparams_tabitem[3])))
        {
            ImGui::Checkbox(u8"Lod Bias", &_enable_lodbias);
            if(_enable_lodbias)
            {
                ImGui::SameLine();
                ImGui::SliderFloat("Bias Value", &_lodbias_value, -10.0f, 10.0f);
            }

            ImGui::Checkbox(u8"Border Color", &_enable_border_color);
            if(_enable_border_color)
            {
                ImGui::SameLine();
                ImGui::ColorEdit4("Color", glm::value_ptr<float32>(_bordercolor));
            }

            ImGui::Checkbox(u8"显示天空盒", &_enable_skybox);
            if (_enable_skybox)
            {
                if(_skybox == nullptr)
                {
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
        ImGui::DragFloat2("Tiling", glm::value_ptr(_tex_tiling), 0.1f, 0.1f, 10.0f);
        ImGui::DragFloat2("Offset", glm::value_ptr(_tex_offset), 0.01f, -1.0f, 1.0f);
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
    //     std::cout << "Selected filename" << fileDialog.GetSelected().string() << std::endl;
    //     fileDialog.ClearSelected();
    // }

    this->SetTexparams();
}

void TextureExploreView::OnWrapModeGUI(int32& wrap_mode_option)
{
    bool wrap_mode_clicked = false;
    ImGui::BeginGroup();
    ImGui::Text(u8"模式:");
    ImGui::Indent();
    wrap_mode_clicked = ImGui::RadioButton(u8"REPEAT", &wrap_mode_option, 0);
    wrap_mode_clicked |= ImGui::RadioButton(u8"CLAMP", &wrap_mode_option, 1);
    wrap_mode_clicked |= ImGui::RadioButton(u8"CLAMP_TO_EDGE", &wrap_mode_option, 2);
    wrap_mode_clicked |= ImGui::RadioButton(u8"CLAMP_TO_BORDER", &wrap_mode_option, 3);
    ImGui::EndGroup();
}

void TextureExploreView::OnFilterModeGUI(int32& filter_mode_option)
{
    bool filter_mode_clicked = false;
    ImGui::BeginGroup();
    ImGui::Text(u8"模式:");
    ImGui::Indent();
    filter_mode_clicked = ImGui::RadioButton(u8"NEAREST", &filter_mode_option, 0);
    filter_mode_clicked |= ImGui::RadioButton(u8"LINEAR", &filter_mode_option, 1);
    filter_mode_clicked |= ImGui::RadioButton(u8"NEAREST_MIPMAP_NEAREST", &filter_mode_option, 2);
    filter_mode_clicked |= ImGui::RadioButton(u8"NEAREST_MIPMAP_LINEAR", &filter_mode_option, 3);
    filter_mode_clicked |= ImGui::RadioButton(u8"LINEAR_MIPMAP_NEAREST", &filter_mode_option, 4);
    filter_mode_clicked |= ImGui::RadioButton(u8"LINEAR_MIPMAP_LINEAR", &filter_mode_option, 5);
    ImGui::EndGroup();
}

void TextureExploreView::OnSwizzleModeGUI()
{
    ImGui::BeginGroup();
    ImGui::Text(u8"模式:");
    ImGui::Indent();

    char const* items[6] = { "RED", "GREEN", "BLUE", "ALPHA", "ZERO", "ONE" };
    if(_swizzle_option == 4 || _swizzle_option == 0)
        ImGui::Combo(u8"Swizzle RED", &(_swizzle_masks[0]), items, 6);

    if(_swizzle_option == 4 || _swizzle_option == 1)
        ImGui::Combo(u8"Swizzle GREEN", &(_swizzle_masks[1]), items, 6);

    if(_swizzle_option == 4 || _swizzle_option == 2)
        ImGui::Combo(u8"Swizzle BLUE", &(_swizzle_masks[2]), items, 6);

    if(_swizzle_option == 4 || _swizzle_option == 3)
        ImGui::Combo(u8"Swizzle ALPHA", &(_swizzle_masks[3]), items, 6);

    ImGui::EndGroup();
}

void TextureExploreView::ResetGUI()
{
    _wrap_option = -1;
    _filter_option = -1;
    _swizzle_option = -1;
    _enable_lodbias = false;
    _enable_border_color = false;
    _wrap_modes[0] = _wrap_modes[1] = _wrap_modes[2] = -1;
    _filter_modes[0] = _filter_modes[1] = -1;
    _swizzle_masks[0] = 0;
    _swizzle_masks[1] = 1;
    _swizzle_masks[2] = 2;
    _swizzle_masks[3] = 3;

    _tintcolor[0] = _tintcolor[1] = _tintcolor[2] = _tintcolor[3] = 1.0f;
    _bordercolor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    _tex_tiling = vec2(1.0f, 1.0f);
    _tex_offset = vec2(0.0f, 0.0f);
}

void TextureExploreView::SetTexparams()
{
    // set wrap mode
    for(int32 i = 0; i < 3; i++)
    {
        WrapMode mode = GetWrapMode(_wrap_modes[i]);
        _texparams.wrap_modes[i] = mode;
    }

    // set filter
    for(int32 i = 0; i < 2; i++)
    {
        FilterMode mode = GetFilterMode(_filter_modes[i]);
        _texparams.filter_modes[i] = mode;
    }

    // set swizzle
    _texparams.swizzle_parameter = GetSwizzleParam(_swizzle_option);
    SwizzleMask masks[4];
    for(int32 i = 0; i < 4; i++)
    {
        _texparams.swizzle[i] = GetSwizzleMask(_swizzle_masks[i]);
    }

    // set lod bias
    _texparams.lod_parameter = _enable_lodbias ? LodBiasParam::LOD_BIAS : LodBiasParam::NONE;
    _texparams.lodbias = _lodbias_value;

    _texparams.bordercolor_parameter = _enable_border_color ? TextureBorderColorParam::BORDER_COLOR : TextureBorderColorParam::NONE;
    _texparams.border_color = _bordercolor;
}

WrapMode TextureExploreView::GetWrapMode(int32 wrap_mode_option)
{
    if(wrap_mode_option == -1)
        return WrapMode::NONE;

    WrapMode modes[5] = { WrapMode::REPEAT, WrapMode::CLAMP, WrapMode::CLAMP_TO_EDGE,
        WrapMode::CLAMP_TO_BORDER, WrapMode::NONE };

    return modes[wrap_mode_option];
}

FilterMode TextureExploreView::GetFilterMode(int32 filter_mode_option)
{
    if(filter_mode_option == -1)
    {
        return FilterMode::NONE;
    }

    // FilterMode modes[7] = { FilterMode::NEAREST, FilterMode::LINEAR, FilterMode::NEAREST_MIPMAP_NEAREST,
    //     FilterMode::NEAREST_MIPMAP_LINEAR, FilterMode::LINEAR_MIPMAP_NEAREST, FilterMode::LINEAR_MIPMAP_LINEAR,
    //     FilterMode::NONE };

    return FilterMode((int)FilterMode::NEAREST + filter_mode_option);// modes[filter_mode_option];
}

SwizzleParam TextureExploreView::GetSwizzleParam(int32 swizzle_option)
{
    if(swizzle_option == -1)
    {
        return SwizzleParam::NONE;
    }

    // SwizzleParam params[5] = { SwizzleParam::SWIZZLE_R, SwizzleParam::SWIZZLE_G, SwizzleParam::SWIZZLE_B,
    //     SwizzleParam::SWIZZLE_A, SwizzleParam::SWIZZLE_RGBA };

    return SwizzleParam((int)SwizzleParam::SWIZZLE_R + swizzle_option);// params[swizzle_option];
}

SwizzleMask TextureExploreView::GetSwizzleMask(int32 swizzle_mask_option)
{
    SwizzleMask masks[6] = { SwizzleMask::RED, SwizzleMask::GREEN, SwizzleMask::BLUE,
        SwizzleMask::ALPHA, SwizzleMask::ZERO, SwizzleMask::ONE };

    return masks[swizzle_mask_option];
}

void TextureExploreView::CreateSprite()
{
    if(_sprite != nullptr) return;
                
    ImageManagerInst imageMgr;
    ImageReadInfo images_info = {"Assets/Textures/test3.png"};
    Image::Ptr image = imageMgr->ReadImage(images_info);

    Texture2D::Ptr texture = nullptr;
    if (image != nullptr)
    {
        texture = std::make_shared<Texture2D>(true);
        texture->SetImage(image);

        // Sampler::Ptr sampler = std::make_shared<Sampler>();
        // texture->SetSampler(sampler);
    }

    // initialise sprite
    if (texture != nullptr)
    {
        _sprite = std::make_shared<Sprite>(texture);
        // Material::Ptr mat = _sprite->GetMaterial();
    }

    CreateGeometry(_sprite, 1);
}

void TextureExploreView::CreateSprite1D()
{
    if(_sprite_1d == nullptr)
    {
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

        GLubyte* bytes = new GLubyte[12]{ 255, 0, 0, 255, 
                                            0, 255, 0, 255, 
                                            0, 0, 255, 255
                                        };
        data.mip[0].data = bytes;

        Texture1D::Ptr texture1D = std::make_shared<Texture1D>(true);
        texture1D->SetImageData(data);

        _sprite_1d = std::make_shared<Sprite>(texture1D, glm::vec2(5, 5));

        CreateGeometry(_sprite_1d, 0);
    }
}

void TextureExploreView::CreateVolumnTexture()
{
    if(_volumn_quad == nullptr)
    {
        vec2 size(10.0f, 10.0f);
        _volumn_quad = std::make_shared<Quad>(size, MeshDataFlag(9));
        _volumn_quad->GenerateMesh();

        MeshRenderer::Ptr renderer = std::make_shared<MeshRenderer>();
        VolumnQuadMaterial::Ptr mat = std::make_shared<VolumnQuadMaterial>();

        ImageManagerInst imageMgr;
        ImageReadInfo images_info = {"Assets/Textures/cloud.dds"};
        Image::Ptr image = imageMgr->ReadImage(images_info);

        Texture3D::Ptr volumntex = std::make_shared<Texture3D>(true);
        volumntex->SetImage(image);

        mat->SetMainTexture(volumntex);
        renderer->SetMaterial(mat);
        renderer->SetMesh(_volumn_quad->GetMesh());

        _volumn_quad->SetMeshRenderer(renderer);

        CreateGeometry(_volumn_quad, 2);
    }
}

void TextureExploreView::CreateSkybox()
{
    if(_skybox == nullptr)
    {
        _skybox = std::make_shared<Cube>(2.0f, MeshDataFlag::DEFAULT);
        _skybox->GenerateMesh();
        
        MeshRenderer::Ptr renderer = std::make_shared<MeshRenderer>();
        SkyboxMaterial::Ptr mat = std::make_shared<SkyboxMaterial>();

        TextureCube::Ptr cubemap = std::make_shared<TextureCube>(true);

        ImageManagerInst imageMgr;
        // ImageReadInfo images_info = {"Assets/Textures/TantolundenCube.dds"};
        // Image::Ptr image = imageMgr->ReadImage(images_info);

        ImageReadInfo front_info = {"Assets/Textures/plains-of-abraham/plains-of-abraham_ft.tga"};
        Image::Ptr front_image = imageMgr->ReadImage(front_info);
        ImageReadInfo back_info = {"Assets/Textures/plains-of-abraham/plains-of-abraham_bk.tga"};
        Image::Ptr back_image = imageMgr->ReadImage(back_info);
        ImageReadInfo left_info = {"Assets/Textures/plains-of-abraham/plains-of-abraham_lf.tga"};
        Image::Ptr left_image = imageMgr->ReadImage(left_info);
        ImageReadInfo right_info = {"Assets/Textures/plains-of-abraham/plains-of-abraham_rt.tga"};
        Image::Ptr right_image = imageMgr->ReadImage(right_info);
        ImageReadInfo top_info = {"Assets/Textures/plains-of-abraham/plains-of-abraham_up.tga"};
        Image::Ptr top_image = imageMgr->ReadImage(top_info);
        ImageReadInfo down_info = {"Assets/Textures/plains-of-abraham/plains-of-abraham_dn.tga"};
        Image::Ptr down_image = imageMgr->ReadImage(down_info);

        // cubemap->SetImage(image);

        cubemap->SetPositiveX(right_image);
        cubemap->SetPositiveY(top_image);
        cubemap->SetPositiveZ(front_image);
        cubemap->SetNegativeX(left_image);
        cubemap->SetNegativeY(down_image);
        cubemap->SetNegativeZ(back_image);
        cubemap->InitStorageByOthers();

        mat->SetMainTexture(cubemap);

        renderer->SetMaterial(mat);
        renderer->SetMesh(_skybox->GetMesh());

        _skybox->SetMeshRenderer(renderer);

        CreateGeometry(_skybox, 3);

        // CreateCube(image);
        // CreateIconSphere(image);
    }
}

void TextureExploreView::CreateCube(Image::Ptr image)
{
    if(_cube == nullptr)
    {
        _cube = std::make_shared<Cube>(5.0f, MeshDataFlag::DEFAULT);
        _cube->GenerateMesh();

        Transform::Ptr trans = _cube->GetTransform();
        trans->Translate(glm::vec3(0.0f, 0.0f, -15.0f));

        trans->SetParent(_camera->GetTransform());

        MeshRenderer::Ptr renderer = std::make_shared<MeshRenderer>();
        CubeMaterial::Ptr mat = std::make_shared<CubeMaterial>();

        TextureCube::Ptr cubemap = std::make_shared<TextureCube>(true);
        cubemap->SetImage(image);

        mat->SetMainTexture(cubemap);

        renderer->SetMaterial(mat);
        renderer->SetMesh(_cube->GetMesh());

        _cube->SetMeshRenderer(renderer);

        CreateGeometry(_cube, 15);
    }
}
void TextureExploreView::CreateIconSphere(Image::Ptr image)
{
    if(_sphere == nullptr)
    {
        _sphere = std::make_shared<IcosahedronSphere>(2.5f, 50, MeshDataFlag::DEFAULT);
        _sphere->GenerateMesh();

        Transform::Ptr trans = _sphere->GetTransform();
        trans->Translate(glm::vec3(-3.5f, 0.0f, 0.0f));
        
        MeshRenderer::Ptr renderer = std::make_shared<MeshRenderer>();
        SphereMaterial::Ptr mat = std::make_shared<SphereMaterial>();

        TextureCube::Ptr cubemap = std::make_shared<TextureCube>(true);
        cubemap->SetImage(image);

        mat->SetMainTexture(cubemap);

        renderer->SetMaterial(mat);
        renderer->SetMesh(_sphere->GetMesh());

        _sphere->SetMeshRenderer(renderer);

        CreateGeometry(_sphere, 14);
    }
}

void TextureExploreView::CreateNURBSSurface()
{
    if(_nurbs_surface == nullptr)
    {
        _nurbs_surface = std::make_shared<NURBSSurface>(5, 3, 5, 3);

        Transform::Ptr trans = _nurbs_surface->GetTransform();
        trans->Translate(glm::vec3(-6.0f, 0.0f, 0.0f));

        glm::vec4* control_points = new glm::vec4[25];

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

        _nurbs_surface->SetControlPoints(control_points, 25);

        Knot* u_knots = new Knot[9];
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
        _nurbs_surface->SetUKnots(u_knots, 9);

        Knot* v_knots = new Knot[9];
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
        _nurbs_surface->SetVKnots(v_knots, 9);

        _nurbs_surface->GenerateMesh();
        Mesh::Ptr mesh = _nurbs_surface->GetMesh();
        SubMesh::Ptr submesh = mesh->GetSubMesh(0);

        MeshRenderer::Ptr renderer = std::make_shared<MeshRenderer>();
        ProjectionMappingMaterial::Ptr mat = std::make_shared<ProjectionMappingMaterial>();

        ImageManagerInst imageMgr;
        ImageReadInfo images_info = {"Assets/Textures/test3.png"};
        Image::Ptr image = imageMgr->ReadImage(images_info);

        Texture2D::Ptr texture = nullptr;
        texture = std::make_shared<Texture2D>(true);
        texture->SetImage(image);

        texture->SetWrap<WrapParam::WRAP_S>(WrapMode::CLAMP_TO_BORDER);
        texture->SetWrap<WrapParam::WRAP_T>(WrapMode::CLAMP_TO_BORDER);

        texture->SetTexBorderColor(TextureBorderColorParam::BORDER_COLOR, vec4(1.0f, 1.0f, 1.0f, 1.0f));

        mat->SetMainTexture(texture);

        renderer->SetSharedMaterial(mat);
        renderer->SetMesh(_nurbs_surface->GetMesh());

        _nurbs_surface->SetMeshRenderer(renderer);

        vec2 size(10.0f, 10.0f);
        _proj_tex_quad = std::make_shared<Quad>(size, MeshDataFlag(9));
        _proj_tex_quad->GenerateMesh();

        Transform::Ptr quad_trans = _proj_tex_quad->GetTransform();
        quad_trans->Translate(glm::vec3(6.0f, 0.0f, 0.0f));

        MeshRenderer::Ptr quad_renderer = std::make_shared<MeshRenderer>();
        ProjectionMappingMaterial::Ptr quad_mat = std::make_shared<ProjectionMappingMaterial>();

        quad_mat->SetMainTexture(texture);
        quad_renderer->SetSharedMaterial(quad_mat);
        quad_renderer->SetMesh(_proj_tex_quad->GetMesh());

        _proj_tex_quad->SetMeshRenderer(quad_renderer);

        CreateGeometry(_nurbs_surface, 6);
        CreateGeometry(_proj_tex_quad, 13);

        SAFE_DEL_ARR(control_points);
        SAFE_DEL_ARR(u_knots);
        SAFE_DEL_ARR(v_knots);
    }
}

void TextureExploreView::RenderGeometryEx(int index)
{
    if(index != 3 && _enable_skybox)
    {
        RenderGeometry(_skybox, 3);
    }

    Geometry::Ptr geom = nullptr;
    switch (index)
    {
    case 0:
        geom = _sprite_1d;
        break;
    case 1:
        geom = _sprite;
        break;
    case 2:
        geom = _volumn_quad;
        break;
    case 3:
        geom = _skybox;
        if(_cube != nullptr)
            RenderGeometry(_cube, 15);
        if(_sphere != nullptr)
            RenderGeometry(_sphere, 14);
        break;
    case 6:
        geom = _nurbs_surface;
        RenderGeometry(_proj_tex_quad, 13);
    default:
        break;
    }

    if(geom != nullptr)
    {
        RenderGeometry(geom, index);
    }
}

void TextureExploreView::CreateGeometry(Geometry::Ptr geom, uint32 index)
{
    SubMesh::Ptr submesh = geom->GetMesh()->GetSubMesh(0);

    //bind element array buffer, bind buffer data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebos[index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, submesh->GetIndiceNum() * 4, submesh->GetIndice(), GL_DYNAMIC_DRAW);

    //bind vertex array object
    glBindVertexArray(_vaos[index]);

    //bind vertex array buffer, bind buffer data
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[index]);

    int32 verticenum = submesh->GetVerticeNum();
    if(geom->GetMeshDataFlag() == MeshDataFlag::DEFAULT)
    {
        glBufferData(GL_ARRAY_BUFFER, submesh->GetVerticeNum() * 12, submesh->GetVerticePos(), GL_DYNAMIC_DRAW);
    }
    else
    {
        // map vertexbuffer data(position & uv)
        glBufferData(GL_ARRAY_BUFFER, submesh->GetDataSize(), nullptr, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, verticenum * 12, submesh->GetVerticePos());

        if((geom->GetMeshDataFlag() & MeshDataFlag::HAS_UV) != 0)
            glBufferSubData(GL_ARRAY_BUFFER, verticenum * 12, verticenum * 16, submesh->GetVerticeUV());
    }    

    // bind vertex position
    glBindVertexBuffer(0, _vbos[index], 0, sizeof(vec3));
    glEnableVertexAttribArray(0);
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);

    // bind vertex uv
    if((geom->GetMeshDataFlag() & MeshDataFlag::HAS_UV) != 0)
    {
        glBindVertexBuffer(1, _vbos[index], verticenum * 12, sizeof(vec4));
        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 4, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(1, 1);
    }
}

void TextureExploreView::RenderGeometry(Geometry::Ptr geom, int32 index, GLenum front_face)
{
    Material::Ptr mat = geom->GetMeshRenderer()->GetMaterial();
    RenderPass::Ptr pass = mat->GetRenderPass(0);
    ShaderProgram::Ptr shader = pass->GetShader();

    for (auto tex_slot : pass->GetTextureSlots())
    {
        tex_slot.second.Apply();
    }

    ShaderProgramUse use(shader);
    for (auto loc : pass->GetUniformLocations())
    {
        loc.second.Bind();
    }

    glEnable(GL_DEPTH_TEST);
    if(index == 3)
        glDepthFunc(GL_LEQUAL);
    else
        glDepthFunc(GL_LESS);

    if(index == 1 || index == 2)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    glFrontFace(front_face);
    glDisable(GL_CULL_FACE);

    SubMesh::Ptr submesh = geom->GetMesh()->GetSubMesh(0);

    //draw command use vertex array object
    glBindVertexArray(_vaos[index]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebos[index]);
    glDrawElements(GL_TRIANGLES, submesh->GetIndiceNum(), GL_UNSIGNED_INT, NULL);

    glFrontFace(GL_CCW);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDisable(GL_BLEND);

    for (auto tex_slot : pass->GetTextureSlots())
    {
        tex_slot.second.UnBind();
    }
}

} // namespace TwinkleGraphics
