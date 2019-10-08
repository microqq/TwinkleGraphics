

#include "imgui.h"
// #include "imgui_filebrowser.h"

#include "twTextureExplore.h"
#include "twImage.h"

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

    // sprite material setting
    Material::Ptr sprite_mat = nullptr;
    if(_sprite != nullptr)
    {
        sprite_mat = _sprite->GetMaterial();
        sprite_mat->SetMatrixUniformValue<float32, 4, 4>("mvp", _mvp_mat);
        vec4 tintColor(_tintcolor[0], _tintcolor[1], _tintcolor[1], _tintcolor[3]);
        sprite_mat->SetVecUniformValue<float32, 4>("tint_color", tintColor);
        sprite_mat->SetTextureTiling("main_tex", _tex_tiling);
        sprite_mat->SetTextureOffset("main_tex", _tex_offset);

        // set sprite texture parameters
        Texture::Ptr tex = _sprite->GetTexture();
        tex->SetWrap<WrapParam::WRAP_S>(_texparams.wrap_modes[0]);
        tex->SetWrap<WrapParam::WRAP_T>(_texparams.wrap_modes[1]);
        tex->SetWrap<WrapParam::WRAP_R>(_texparams.wrap_modes[2]);

        tex->SetFilter<FilterParam::MIN_FILTER>(_texparams.filter_modes[0]);
        tex->SetFilter<FilterParam::MAG_FILTER>(_texparams.filter_modes[1]);

        tex->SetTexBorderColor(_texparams.bordercolor_parameter, _texparams.border_color);
    }
}
void TextureExploreView::RenderImpl()
{
    switch (_current_tex_option)
    {
    case 0:
        /* code */
        break;
    case 1:
        RenderSprite();
        break;
    case 2:
        /* code */
        break;
    default:
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
        }
        if(ImGui::RadioButton(u8"二维纹理", &_current_tex_option, 1))
        {
            ResetGUI();
            if(_sprite == nullptr)
            {
                CreateSprite();
            }
        }
        if(ImGui::RadioButton(u8"三维纹理", &_current_tex_option, 2))
        {
            ResetGUI();
        }
        if(ImGui::RadioButton(u8"立方体纹理", &_current_tex_option, 3))
        {
            ResetGUI();

        }
        if(ImGui::RadioButton(u8"程序纹理", &_current_tex_option, 4))
        {
            ResetGUI();

        }
        if(ImGui::RadioButton(u8"法线纹理", &_current_tex_option, 5))
        {
            ResetGUI();

        }
        if(ImGui::RadioButton(u8"投影纹理", &_current_tex_option, 6))
        {
            ResetGUI();

        }
        if(ImGui::RadioButton(u8"纹理视图", &_current_tex_option, 7))
        {
            ResetGUI();

        }
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

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::End();

    ImGui::Begin(u8"其他...");
    {
        ImGui::DragFloat2("Tiling", glm::value_ptr(_tex_tiling), 0.1f, 0.1f, 10.0f);
        ImGui::DragFloat2("Offset", glm::value_ptr(_tex_offset), 0.01f, -1.0f, 1.0f);

        if (_current_tex_option == 1)
        {
            ImGui::ColorEdit4("Tint Color", _tintcolor);
        }
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

    FilterMode modes[7] = { FilterMode::NEAREST, FilterMode::LINEAR, FilterMode::NEAREST_MIPMAP_NEAREST,
        FilterMode::NEAREST_MIPMAP_LINEAR, FilterMode::LINEAR_MIPMAP_NEAREST, FilterMode::LINEAR_MIPMAP_LINEAR,
        FilterMode::NONE };

    return modes[filter_mode_option];
}

SwizzleParam TextureExploreView::GetSwizzleParam(int32 swizzle_option)
{
    if(swizzle_option == -1)
    {
        return SwizzleParam::NONE;
    }

    SwizzleParam params[5] = { SwizzleParam::SWIZZLE_R, SwizzleParam::SWIZZLE_G, SwizzleParam::SWIZZLE_B,
        SwizzleParam::SWIZZLE_A, SwizzleParam::SWIZZLE_RGBA };

    return params[swizzle_option];
}

SwizzleMask TextureExploreView::GetSwizzleMask(int32 swizzle_mask_option)
{
    SwizzleMask masks[6] = { SwizzleMask::RED, SwizzleMask::GREEN, SwizzleMask::BLUE,
        SwizzleMask::ALPHA, SwizzleMask::ZERO, SwizzleMask::ONE };

    return masks[swizzle_mask_option];
}

void TextureExploreView::CreateSprite()
{
    ImageManagerInst imageMgr;
    ImageReadInfo images_info[] = {{"Assets/Textures/test.dds"}};
    Image::Ptr image = imageMgr->ReadImage(images_info[0]);

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
        // Material::Ptr sprite_mat = _sprite->GetMaterial();
    }

    CreateGeometry(_sprite, 1);
}

void TextureExploreView::RenderSprite()
{
    if(_sprite != nullptr)
    {
        Material::Ptr sprite_mat = _sprite->GetMaterial();
        RenderPass::Ptr pass = sprite_mat->GetRenderPass(0);
        ShaderProgram::Ptr shader = pass->GetShader();

        for(auto tex_slot : pass->GetTextureSlots())
        {
            tex_slot.second.Apply();
        }

        ShaderProgramUse use(shader);
        for(auto loc : pass->GetUniformLocations())
        {
            loc.second.Bind();
        }

        RenderGeometry(_sprite, 1);
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

    int32 verticenum =submesh->GetVerticeNum();
    glBufferData(GL_ARRAY_BUFFER, submesh->GetDataSize(), nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, verticenum * 12, submesh->GetVerticePos());
    glBufferSubData(GL_ARRAY_BUFFER, verticenum * 12, verticenum * 16, submesh->GetVerticeUV());

    glBindVertexBuffer(0, _vbos[index], 0, sizeof(vec3));
    glEnableVertexAttribArray(0);
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);

    glBindVertexBuffer(1, _vbos[index], verticenum * 12, sizeof(vec4));
    glEnableVertexAttribArray(1);
    glVertexAttribFormat(1, 4, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(1, 1);
}

void TextureExploreView::RenderGeometry(Geometry::Ptr geom, int32 index, GLenum front_face)
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glFrontFace(front_face);
    glDisable(GL_CULL_FACE);

    SubMesh::Ptr submesh = geom->GetMesh()->GetSubMesh(0);

    //draw command use vertex array object
    glBindVertexArray(_vaos[index]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebos[index]);
    glDrawElements(GL_TRIANGLES, submesh->GetIndiceNum(), GL_UNSIGNED_INT, NULL);

    glFrontFace(GL_CCW);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

} // namespace TwinkleGraphics
