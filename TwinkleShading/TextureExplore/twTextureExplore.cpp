

#include "imgui.h"

#include "twTextureExplore.h"
#include "twImage.h"

#include "twRenderContext.h"

namespace TwinkleGraphics
{
TextureExplore::TextureExplore(std::string& name)
    : GLPlugin(name)
    , _view(nullptr)
    , _camera_control(nullptr)
{}

TextureExplore::~TextureExplore()
{
    SAFE_DEL(_view);
    SAFE_DEL(_camera_control);
}

void TextureExplore::Install()
{
    Plugin::Install();

    // Initilize view
    Viewport viewport(Rect(0, 0, 800, 640), 17664U, RGBA(0.0f, 0.f, 0.f, 1.f));
    Camera::Ptr camera = std::make_shared<Camera>(viewport, 45.0f, 0.1f, 1000.0f);
    _view = new TextureExploreView();
    _view->SetViewCamera(camera);
    _camera_control = new OrbitControl(camera);
    (dynamic_cast<OrbitControl*>(_camera_control))->SetMinDistance(1.0f);
    _view->SetCameraControl(_camera_control);
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
    ImageManagerInst imageMgr;
    ImageReadInfo images_info[] =
    {
        {"Assets/Textures/test.dds"}
    };
    Image::Ptr image = imageMgr->ReadImage(images_info[0]);

    Texture2D::Ptr texture = nullptr;
    if(image != nullptr)
    {
        texture = std::make_shared<Texture2D>(true);
        texture->SetImage(image);
    }

    // initialise sprite
    if(texture != nullptr)
    {
        _sprite = std::make_shared<Sprite>(texture);
    }

    ShaderManagerInst shaderMgr;

    ShaderReadInfo sprite_shader_info[] = {
        {std::string("Assets/Shaders/sprite.vert"), ShaderType::VERTEX_SHADER},
        {std::string("Assets/Shaders/sprite.frag"), ShaderType::FRAGMENT_SHADER}};
    ShaderProgram::Ptr sprite_shader = shaderMgr->ReadShaders(sprite_shader_info, 2);

    RenderPass::Ptr pass0 = std::make_shared<RenderPass>(sprite_shader);
    Material::Ptr material = std::make_shared<Material>();
    material->AddRenderPass(pass0);

    vec4 tint_color;
    material->SetMainTexture(texture);
    material->SetVecUniformValue<float32, 4>("tint_color", tint_color);
}
void TextureExploreView::Destroy()
{
}
void TextureExploreView::Advance(float64 delta_time)
{
}
void TextureExploreView::RenderImpl()
{
}
void TextureExploreView::OnGUI()
{
    ImGui::Begin(u8"纹理小观");
    {
        if(ImGui::RadioButton(u8"一维纹理", &_current_tex_option, 0))
        {

        }
        if(ImGui::RadioButton(u8"二维纹理", &_current_tex_option, 1))
        {

        }
        if(ImGui::RadioButton(u8"三维纹理", &_current_tex_option, 2))
        {

        }
        if(ImGui::RadioButton(u8"立方体纹理", &_current_tex_option, 3))
        {

        }
        if(ImGui::RadioButton(u8"程序纹理", &_current_tex_option, 4))
        {

        }
        if(ImGui::RadioButton(u8"法线纹理", &_current_tex_option, 5))
        {

        }
        if(ImGui::RadioButton(u8"投影纹理", &_current_tex_option, 6))
        {

        }
        if(ImGui::RadioButton(u8"纹理视图", &_current_tex_option, 7))
        {

        }
    }
    ImGui::End();
}

void TextureExploreView::RenderSprite()
{
    if(_sprite != nullptr)
    {
        
    }
}

} // namespace TwinkleGraphics
