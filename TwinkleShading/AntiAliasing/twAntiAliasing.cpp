

#include "imgui.h"

#include "twAntiAliasing.h"
#include "twImage.h"
#include "twRenderTexture.h"

namespace TwinkleGraphics
{
AntiAliasing::AntiAliasing(std::string& name)
    : GLPlugin(name)
    , _view(nullptr)
    // , _view2(nullptr)
{}

AntiAliasing::~AntiAliasing()
{
    SAFE_DEL(_view);
    // SAFE_DEL(_view2);
}

void AntiAliasing::Install()
{
    Plugin::Install();

    // create view
    _view = new AntiAliasingView();
    // _view2 = new AntiAliasingView();

    Viewport viewport(Rect(0, 0, 1024, 768), 17664U, RGBA(0.0f, 0.f, 0.0f, 1.f));
    Camera::Ptr camera = std::make_shared<Camera>(viewport, 45.0f, 0.1f, 1000.0f);
    _view->SetViewCamera(camera);

    OrbitControl::Ptr camera_control = std::make_shared<OrbitControl>(camera);
    camera_control->SetMinDistance(1.0f);
    _view->SetCameraControl(camera_control);

    AntiAliasingScene::Ptr scene = std::make_shared<AntiAliasingScene>();
    _view->AttachToScene(scene);

    _views[_views_count++] = _view;
    // _views[_views_count++] = _view2;
}

void AntiAliasing::UnInstall()
{
    _view->Destroy();
    SAFE_DEL(_view);

    Plugin::UnInstall();
}

void AntiAliasing::UpdateViews()
{
    const glm::ivec2& size = _view->GetWindowSize();
    _view->ResetViewport(Rect(0, 0, size.x, size.y));
    // float32 ratio = (float32)size.x / (float32)size.y;
    // int32 w = ratio * 200.0f;
    // _view2->ResetViewport(Rect(size.x - w, 0, w, 200.0f));
}

void AntiAliasingView::Initialize()
{
    if(_initialized)
        return;
    if(_scene != nullptr)
    {
        _scene->Init();
    }

    View::Initialize();
}

void AntiAliasingView::Destroy()
{
    dynamic_cast<AntiAliasingScene*>(_scene.get())->DestroyScene();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);    
}

void AntiAliasingView::OnGUI()
{
    ImGui::Begin(u8"反走样");
    {
        int32& current_aa_option = dynamic_cast<AntiAliasingScene*>(_scene.get())->GetCurrentAAOption();

        if (ImGui::RadioButton(u8"NO AA", &(current_aa_option), AAOption::NONE))
        {
        }
        if (ImGui::RadioButton(u8"MSAA_HW", &(current_aa_option), AAOption::MSAA_HW))
        {
        }
        if (ImGui::RadioButton(u8"MSAA_SW", &current_aa_option, AAOption::MSAA_SW))
        {
        }
        if (ImGui::RadioButton(u8"SSAA", &current_aa_option, AAOption::SSAA))
        {
        }
        if (ImGui::RadioButton(u8"CSAA", &current_aa_option, AAOption::CSAA))
        {
        }
        if (ImGui::RadioButton(u8"CFAA", &current_aa_option, AAOption::CFAA))
        {
        }
        if (ImGui::RadioButton(u8"FXAA", &current_aa_option, AAOption::FXAA))
        {
        }
    }
    ImGui::End();
}

void AntiAliasingScene::CreateScene()
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
        cubemap->CreateFromImage(image);

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

    glm::vec4 tintcolor(0.6f, 0.9f, 0.2f, 1.0f);
    glm::vec3 p0(-3.0f, 5.0f, 0.0f), p1(-4.0f, 0.0f, 0.0f), p2(3.0f, 5.2f, 0.0f);
    _triangle_back.reset(CreateTriangle(p0, p1, p2));
    {
        Material::Ptr material = _triangle_back->GetMeshRenderer()->GetSharedMaterial();
        material->SetVecUniformValue<float32, 4>("tint_color", tintcolor);

        CreateGeometry(_triangle_back, 7);
    }
    p0 = glm::vec3(-2.0f, 3.0f, 0.0f);
    p1 = glm::vec3(-3.0f, 0.0f, 0.0f);
    p2 = glm::vec3(3.0f, 2.3f, 0.0f);
    _triangle_front.reset(CreateTriangle(p0, p1, p2));
    {
        tintcolor = glm::vec4(1.0f, 0.3f, 0.8f, 1.0f);
        Material::Ptr material = _triangle_front->GetMeshRenderer()->GetSharedMaterial();
        material->SetVecUniformValue<float32, 4>("tint_color", tintcolor);

        CreateGeometry(_triangle_front, 8);
    }

    //
    RenderTexture::Ptr rt = std::make_shared<RenderTexture>(256, 256, GL_RGBA8
        , GL_RGBA, true, false, true, 4, true);
    rt->Create(true);
}

void AntiAliasingScene::UpdateScene()
{
    const Viewport& viewport = _maincamera->GetViewport();
    _viewport_params = glm::vec4((float32)(viewport.Width()), (float32)(viewport.Height()), viewport.AspectRatio(), 1.0f);
    _view_mat = _maincamera->GetViewMatrix();
    _projection_mat = _maincamera->GetProjectionMatrix();
    _mvp_mat = _projection_mat * _view_mat;

    Material::Ptr material_plane_left = _plane_left->GetMeshRenderer()->GetSharedMaterial();
    glm::mat4 model = _plane_left->GetTransform()->GetLocalToWorldMatrix();
    glm::mat4 mvp = _mvp_mat * model;
    material_plane_left->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
    glm::vec4 tint_color(0.5f, 0.0f, 0.0f, 1.0f);
    material_plane_left->SetVecUniformValue<float32, 4>("tint_color", tint_color);

    Material::Ptr material_plane_top = _plane_top->GetMeshRenderer()->GetSharedMaterial();
    {
        model = _plane_top->GetTransform()->GetLocalToWorldMatrix();
        mvp = _mvp_mat * model;
        material_plane_top->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
        tint_color = glm::vec4(0.0f, 0.0f, 0.5f, 1.0f);
        material_plane_top->SetVecUniformValue<float32, 4>("tint_color", tint_color);
    }

    Material::Ptr material_plane_right = _plane_right->GetMeshRenderer()->GetSharedMaterial();
    {
        model = _plane_right->GetTransform()->GetLocalToWorldMatrix();
        mvp = _mvp_mat * model;
        material_plane_right->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
        tint_color = glm::vec4(0.0f, 0.5f, 0.0f, 1.0f);
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
        tint_color = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
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
        spheretex->SetFilter<FilterParam::MAG_FILTER>(FilterMode::LINEAR);
    }

    Material::Ptr material_cube = _cube->GetMeshRenderer()->GetSharedMaterial();
    {
        Transform::Ptr trans = _cube->GetTransform();
        trans->Rotate(0.002f, glm::vec3(0.0f, 1.0f, 0.0f));

        mvp = _mvp_mat * trans->GetLocalToWorldMatrix();
        material_cube->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
    }

    Material::Ptr material_triangle = _triangle_back->GetMeshRenderer()->GetSharedMaterial();
    {
        material_triangle->SetMatrixUniformValue<float32, 4, 4>("mvp", _mvp_mat);
    }
    material_triangle = _triangle_front->GetMeshRenderer()->GetSharedMaterial();
    {
        material_triangle->SetMatrixUniformValue<float32, 4, 4>("mvp", _mvp_mat);
    }

    _root_trans->Rotate(0.001f, glm::vec3(0.0f, 1.0f, 0.0f));
}

void AntiAliasingScene::RenderScene()
{
    switch (_current_aa_option)
    {
    case AAOption::MSAA_HW:    
        glEnable(GL_MULTISAMPLE);
        break;
    default:
        glDisable(GL_MULTISAMPLE);
        break;
    }

    RenderGeometry(_plane_left, 0);
    RenderGeometry(_plane_top, 1);
    RenderGeometry(_plane_right, 2);
    RenderGeometry(_plane_bottom, 3);
    RenderGeometry(_plane_back, 4);
    RenderGeometry(_sphere, 5);
    RenderGeometry(_cube, 6);
    RenderGeometry(_triangle_back, 7);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(-1.0f, 0.0f);
    RenderGeometry(_triangle_front, 8);
    glDisable(GL_POLYGON_OFFSET_FILL);
}

void AntiAliasingScene::DestroyScene()
{
    ShaderProgramUse use(nullptr);

    //unbind vao/ebo
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //Delete buffers
    glDeleteVertexArrays(16, _vaos);
    glDeleteBuffers(16, _vbos);
    glDeleteBuffers(16, _ebos);
}


void AntiAliasingScene::CreateGeometry(Geometry::Ptr geom, uint32 index)
{
    SubMesh::Ptr submesh = geom->GetMesh()->GetSubMesh(0);

    //bind vertex array object
    glBindVertexArray(_vaos[index]);

    //bind element array buffer, bind buffer data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebos[index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, submesh->GetIndiceNum() * 4, submesh->GetIndice(), GL_DYNAMIC_DRAW);

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
    glEnableVertexAttribArray(0);
    glBindVertexBuffer(0, _vbos[index], 0, sizeof(vec3));
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);

    // bind vertex uv
    if((geom->GetMeshDataFlag() & MeshDataFlag::HAS_UV) != 0)
    {
        glEnableVertexAttribArray(1);
        glBindVertexBuffer(1, _vbos[index], verticenum * 12, sizeof(vec4));
        glVertexAttribFormat(1, 4, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(1, 1);
    }

    glBindVertexArray(0);
}

void AntiAliasingScene::RenderGeometry(Geometry::Ptr geom, int32 index)
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
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    SubMesh::Ptr submesh = geom->GetMesh()->GetSubMesh(0);

    //draw command use vertex array object
    glBindVertexArray(_vaos[index]);
    glDrawElements(GL_TRIANGLES, submesh->GetIndiceNum(), GL_UNSIGNED_INT, NULL);

    for (auto tex_slot : pass->GetTextureSlots())
    {
        tex_slot.second.UnBind();
    }
}

} // namespace TwinkleGraphics
