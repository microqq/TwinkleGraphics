

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

    Material::Ptr sprite_mat = nullptr;
    if(_sprite != nullptr)
    {
        sprite_mat = _sprite->GetMaterial();
        sprite_mat->SetMatrixUniformValue<float32, 4, 4>("mvp", _mvp_mat);
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

        }
        if(ImGui::RadioButton(u8"二维纹理", &_current_tex_option, 1))
        {
            if(_sprite == nullptr)
            {
                CreateSprite();
            }
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

    ImGui::Begin(u8"纹理参数");
    {

    }
    ImGui::End();
}

void TextureExploreView::CreateSprite()
{
    ImageManagerInst imageMgr;
    ImageReadInfo images_info[] =
        {
            {"Assets/Textures/test.dds"}};
    Image::Ptr image = imageMgr->ReadImage(images_info[0]);

    Texture2D::Ptr texture = nullptr;
    if (image != nullptr)
    {
        texture = std::make_shared<Texture2D>(true);
        texture->SetImage(image);
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
    glBufferSubData(GL_ARRAY_BUFFER, verticenum * 12, verticenum * 16, submesh->GetVerticePos());

    glBindVertexBuffer(0, _vbos[index], 0, sizeof(vec3));
    glEnableVertexAttribArray(0);
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);

    glBindVertexBuffer(1, _vbos[index], verticenum * 12, sizeof(vec4));
    glEnableVertexAttribArray(1);
    glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
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
    // glDrawElements(GL_POINTS, submesh->GetIndiceNum(), GL_UNSIGNED_INT, NULL);


    glFrontFace(GL_CCW);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

} // namespace TwinkleGraphics
