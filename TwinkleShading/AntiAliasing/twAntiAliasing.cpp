

#include "imgui.h"

#include "twAntiAliasing.h"
#include "twImage.h"

namespace TwinkleGraphics
{
AntiAliasing::AntiAliasing(std::string& name)
    : GLPlugin(name)
    , _view(nullptr)
    , _camera_control(nullptr)
    , _view2(nullptr)
    , _camera_control2(nullptr)
{}

AntiAliasing::~AntiAliasing()
{
    SAFE_DEL(_view);
    SAFE_DEL(_camera_control);
    SAFE_DEL(_view2);
    SAFE_DEL(_camera_control2);
}

void AntiAliasing::Install()
{
    Plugin::Install();

    // Initilize view
    Viewport viewport(Rect(0, 0, 800, 640), 17664U, RGBA(0.0f, 0.f, 0.f, 1.f));
    Camera::Ptr camera = std::make_shared<Camera>(viewport, 45.0f, 0.1f, 1000.0f);
    _view = new AntiAliasingView();

    _view->SetViewCamera(camera);
    _camera_control = new OrbitControl(camera);
    (dynamic_cast<OrbitControl*>(_camera_control))->SetMinDistance(1.0f);
    _view->SetCameraControl(_camera_control);
    _view->Initialize();

    // Viewport viewport2(Rect(400, 0, 400, 640), 17664U, RGBA(0.0f, 0.f, 0.f, 1.f));
    // camera = std::make_shared<Camera>(viewport2, 45.0f, 0.1f, 1000.0f);
    // _view2 = new AntiAliasingView();
    // _view2->SetViewCamera(camera);
    // _camera_control2 = new OrbitControl(camera);
    // _view2->SetCameraControl(_camera_control2);
    // _view2->Initialize();

    _views[_views_count++] = _view;
    // _views[_views_count++] = _view2;
}

void AntiAliasing::UnInstall()
{
    _view->Destroy();
    SAFE_DEL(_view);

    Plugin::UnInstall();
}

void AntiAliasingView::Initialize()
{
    //create vertex buffer object
    _vbos = new uint32[16];
    glGenBuffers(16, _vbos);
    _ebos = new uint32[16];
    glGenBuffers(16, _ebos);

    //create vertex array object
    _vaos = new uint32[16];
    glGenVertexArrays(16, _vaos);

    CreateScene();

    _view_mat = _camera->GetViewMatrix();
    _projection_mat = _camera->GetProjectionMatrix();
    _origin_mvp_mat = _projection_mat * _view_mat;
}

void AntiAliasingView::Destroy()
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

void AntiAliasingView::Advance(float64 delta_time)
{
    UpdateScene();
}

void AntiAliasingView::RenderImpl()
{
    RenderScene();
}

void AntiAliasingView::OnGUI()
{
    ImGui::Begin(u8"反走样");
    {
        if (ImGui::RadioButton(u8"MSAA", &_current_aa_option, 0))
        {
        }
    }
    ImGui::End();
}

void AntiAliasingView::CreateScene()
{
    _plane_left.reset(CreatePlane(glm::vec3(1.0f, 0.0f, 0.0f), 30.0f));
    Transform::Ptr trans = _plane_left->GetTransform();
    {
        trans->Translate(glm::vec3(-15.0f, 0.0f, 0.0f));
        CreateGeometry(_plane_left, 0);
    }

    _plane_top.reset(CreatePlane(glm::vec3(0.0f, -1.0f, 0.0f), 30.0f));
    {
        trans = _plane_top->GetTransform();
        trans->Translate(glm::vec3(0.0f, 15.0f, 0.0f));
        CreateGeometry(_plane_top, 1);
    }

    _plane_right.reset(CreatePlane(glm::vec3(1.0f, 0.0f, 0.0f), 30.0f));
    {
        trans = _plane_right->GetTransform();
        trans->Translate(glm::vec3(15.0f, 0.0f, 0.0f));
        CreateGeometry(_plane_right, 2);
    }

    _plane_bottom.reset(CreatePlane(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f));
    {
        trans = _plane_bottom->GetTransform();
        trans->Translate(glm::vec3(0.0f, -15.0f, 0.0f));
        CreateGeometry(_plane_bottom, 3);
    }

    _plane_back.reset(CreatePlane(glm::vec3(0.0f, 0.0f, 1.0f), 30.0f));
    {
        trans = _plane_back->GetTransform();
        trans->Translate(glm::vec3(0.0f, 0.0f, -15.0f));
        CreateGeometry(_plane_back, 4);
    }

    _root_trans = std::make_shared<Transform>();

    _sphere.reset(CreateIcosahedronSphere(3.0f, 10, MeshDataFlag(MeshDataFlag::HAS_UV)));
    {
        trans = _sphere->GetTransform();
        trans->Translate(glm::vec3(-5.0f, -5.0f, 0.0f));
        trans->SetParent(_root_trans);

        CreateGeometry(_sphere, 5);

        ImageManagerInst imageMgr;
        ImageReadInfo images_info = {"Assets/Textures/TantolundenCube.dds"};
        Image::Ptr image = imageMgr->ReadImage(images_info);
        TextureCube::Ptr cubemap = std::make_shared<TextureCube>(true);
        cubemap->SetImage(image);

        Material::Ptr mat = _sphere->GetMeshRenderer()->GetSharedMaterial();
        mat->SetMainTexture(cubemap);
    }

    _cube.reset(CreateCube(5.0f));
    {
        trans = _cube->GetTransform();
        trans->Translate(glm::vec3(5.0f, -5.0f, 0.0f));
        trans->SetParent(_root_trans);

        CreateGeometry(_cube, 6);
    }
}

void AntiAliasingView::UpdateScene()
{
    const Viewport& viewport = _camera->GetViewport();
    _viewport_params = glm::vec4((float32)(viewport.Width()), (float32)(viewport.Height()), viewport.AspectRatio(), 1.0f);
    _view_mat = _camera->GetViewMatrix();
    _projection_mat = _camera->GetProjectionMatrix();
    _mvp_mat = _projection_mat * _view_mat;

    Material::Ptr material_plane_left = _plane_left->GetMeshRenderer()->GetSharedMaterial();
    glm::mat4 model = _plane_left->GetTransform()->GetLocalToWorldMatrix();
    glm::mat4 mvp = _mvp_mat * model;
    material_plane_left->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
    glm::vec4 tint_color(0.7f, 0.0f, 0.0f, 1.0f);
    material_plane_left->SetVecUniformValue<float32, 4>("tint_color", tint_color);

    Material::Ptr material_plane_top = _plane_top->GetMeshRenderer()->GetSharedMaterial();
    {
        model = _plane_top->GetTransform()->GetLocalToWorldMatrix();
        mvp = _mvp_mat * model;
        material_plane_top->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
        tint_color = glm::vec4(0.0f, 0.0f, 0.7f, 1.0f);
        material_plane_top->SetVecUniformValue<float32, 4>("tint_color", tint_color);
    }

    Material::Ptr material_plane_right = _plane_right->GetMeshRenderer()->GetSharedMaterial();
    {
        model = _plane_right->GetTransform()->GetLocalToWorldMatrix();
        mvp = _mvp_mat * model;
        material_plane_right->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
        tint_color = glm::vec4(0.0f, 0.7f, 0.0f, 1.0f);
        material_plane_right->SetVecUniformValue<float32, 4>("tint_color", tint_color);
    }

    Material::Ptr material_plane_bottom = _plane_bottom->GetMeshRenderer()->GetSharedMaterial();
    {
        model = _plane_bottom->GetTransform()->GetLocalToWorldMatrix();
        mvp = _mvp_mat * model;
        material_plane_bottom->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
        tint_color = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
        material_plane_bottom->SetVecUniformValue<float32, 4>("tint_color", tint_color);
    }

    Material::Ptr material_plane_back = _plane_back->GetMeshRenderer()->GetSharedMaterial();
    {
        model = _plane_back->GetTransform()->GetLocalToWorldMatrix();
        tint_color = glm::vec4(0.7f, 0.7f, 0.0f, 1.0f);
        mvp = _mvp_mat * model;
        material_plane_back->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
        material_plane_back->SetVecUniformValue<float32, 4>("tint_color", tint_color);
    }

    Material::Ptr material_sphere = _sphere->GetMeshRenderer()->GetSharedMaterial();
    {
        Transform::Ptr trans = _sphere->GetTransform();
        trans->Rotate(0.002f, glm::vec3(0.0f, 1.0f, 0.0f));
        mvp = _mvp_mat * trans->GetLocalToWorldMatrix();
        material_sphere->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);

        Texture::Ptr spheretex = material_sphere->GetMainTexture();
        spheretex->SetWrap<WrapParam::WRAP_S>(WrapMode::CLAMP_TO_EDGE);
        spheretex->SetWrap<WrapParam::WRAP_T>(WrapMode::CLAMP_TO_EDGE);
        spheretex->SetWrap<WrapParam::WRAP_R>(WrapMode::CLAMP_TO_EDGE);

        spheretex->SetFilter<FilterParam::MIN_FILTER>(FilterMode::LINEAR_MIPMAP_LINEAR);
        spheretex->SetFilter<FilterParam::MAG_FILTER>(FilterMode::LINEAR_MIPMAP_LINEAR);
    }

    Material::Ptr material_cube = _cube->GetMeshRenderer()->GetSharedMaterial();
    {
        Transform::Ptr trans = _cube->GetTransform();
        trans->Rotate(0.002f, glm::vec3(0.0f, 1.0f, 0.0f));

        mvp = _mvp_mat * trans->GetLocalToWorldMatrix();
        material_cube->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
    }

    _root_trans->Rotate(0.001f, glm::vec3(0.0f, 1.0f, 0.0f));
}

void AntiAliasingView::RenderScene()
{
    RenderGeometry(_plane_left, 0);
    RenderGeometry(_plane_top, 1);
    RenderGeometry(_plane_right, 2);
    RenderGeometry(_plane_bottom, 3);
    RenderGeometry(_plane_back, 4);
    RenderGeometry(_sphere, 5);
    RenderGeometry(_cube, 6);
}

void AntiAliasingView::DestroyScene()
{

}


void AntiAliasingView::CreateGeometry(Geometry::Ptr geom, uint32 index)
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

void AntiAliasingView::RenderGeometry(Geometry::Ptr geom, int32 index)
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

    glFrontFace(GL_CCW);
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    SubMesh::Ptr submesh = geom->GetMesh()->GetSubMesh(0);

    //draw command use vertex array object
    glBindVertexArray(_vaos[index]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebos[index]);
    glDrawElements(GL_TRIANGLES, submesh->GetIndiceNum(), GL_UNSIGNED_INT, NULL);

    glDisable(GL_BLEND);

    for (auto tex_slot : pass->GetTextureSlots())
    {
        tex_slot.second.UnBind();
    }
}

} // namespace TwinkleGraphics
