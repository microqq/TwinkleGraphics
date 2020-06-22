

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
    _view->AttachScene(scene);

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
    const glm::ivec2& size = _view->GetViewSize();
    Rect rect(0, 0, size.x, size.y);
    _view->ResetViewport(rect);
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
        AntiAliasingScene* scene = dynamic_cast<AntiAliasingScene*>(_scene.get());
        int32& current_aa_option = scene->GetCurrentAAOption();
        int32& current_msaa_sw_option = scene->_current_msaa_sw_option;

        ImGui::RadioButton(u8"NO AA", &(current_aa_option), AAOption::NONE);
        ImGui::RadioButton(u8"MSAA_HW", &(current_aa_option), AAOption::MSAA_HW);
        ImGui::RadioButton(u8"MSAA_SW", &current_aa_option, AAOption::MSAA_SW);
        if(current_aa_option == AAOption::MSAA_SW)
        {
            ImGui::BeginGroup();
            ImGui::Indent();
            ImGui::Checkbox(u8"多重采样", &(scene->_enable_multisample));
            ImGui::RadioButton(u8"Back Buffer", &(current_msaa_sw_option), MSAASWOption::MSAA_SW_BACKBUFFER);
            ImGui::RadioButton(u8"Screen FBO", &(current_msaa_sw_option), MSAASWOption::MSAA_SW_SCREENFBO);
            ImGui::RadioButton(u8"Resolve Filter", &(current_msaa_sw_option), MSAASWOption::MSAA_SW_RESOLVE);
            if(current_msaa_sw_option == MSAASWOption::MSAA_SW_RESOLVE)
            {
                MeshRenderer::Ptr renderer = scene->_screen_quad_msaa->GetMeshRenderer();
                renderer->SetSharedMaterial(scene->_msaa_resolve_mat);
            }
            else
            {
                MeshRenderer::Ptr renderer = scene->_screen_quad_msaa->GetMeshRenderer();
                renderer->SetSharedMaterial(scene->_screen_quad_mat);
            }
            ImGui::EndGroup();
        }

        ImGui::RadioButton(u8"SSAA", &current_aa_option, AAOption::SSAA);
        ImGui::RadioButton(u8"CSAA", &current_aa_option, AAOption::CSAA);
        ImGui::RadioButton(u8"CFAA", &current_aa_option, AAOption::CFAA);
        ImGui::RadioButton(u8"FXAA", &current_aa_option, AAOption::FXAA);
    }
    ImGui::End();
}

void AntiAliasingScene::CreateScene()
{
    // create conel box
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

    // icosphere
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

    // cube
    _cube.reset(CreateCube(5.0f));
    {
        trans = _cube->GetTransform();
        trans->Translate(glm::vec3(5.0f, -5.0f, 0.0f));
        trans->SetParent(_root_trans);

        CreateGeometry(_cube, 6);
    }

    // triangles
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

    // rt for msaa resolve fitler 
    _rt_msaa = std::make_shared<RenderTexture>(1024, 768, GL_RGBA8
        , GL_RGBA, true, false, true, 4, true);
    _rt_msaa->Create(nullptr);

    _rt_msaa_resolve = std::make_shared<RenderTexture>(1024, 768, GL_RGBA8
        , GL_RGBA, false, false, false, 4, false);
    _rt_msaa_resolve->Create(nullptr);
    _rt_msaa_resolve->GetTexture()->SetFilter<FilterParam::MIN_FILTER>(FilterMode::LINEAR);
    _rt_msaa_resolve->GetTexture()->SetFilter<FilterParam::MAG_FILTER>(FilterMode::LINEAR);

    vec2 size(2, 2);
    _screen_quad_msaa = std::make_shared<Quad>(size, MeshDataFlag(8));
    _screen_quad_msaa->GenerateMesh();
    {
        CreateGeometry(_screen_quad_msaa, 9);
    }

    // create screen quad material
    ShaderManagerInst shaderMgr;
    ShaderReadInfo shader_info[] = {
        {std::string("Assets/Shaders/screenquad.vert"), ShaderType::VERTEX_SHADER},
        {std::string("Assets/Shaders/screenquad.frag"), ShaderType::FRAGMENT_SHADER}};
    ShaderProgram::Ptr program = shaderMgr->ReadShaders(shader_info, 2);
    RenderPass::Ptr pass = std::make_shared<RenderPass>(program);

    _screen_quad_mat = std::make_shared<Material>();
    _screen_quad_mat->AddRenderPass(pass);

    _screen_quad_mat->SetMainTexture(_rt_msaa_resolve->GetTexture());
    MeshRenderer::Ptr quad_renderer = std::make_shared<MeshRenderer>();
    quad_renderer->SetSharedMaterial(_screen_quad_mat);
    quad_renderer->SetMesh(_screen_quad_msaa->GetMesh());
    _screen_quad_msaa->SetMeshRenderer(quad_renderer);


    // create material for custom msaa resolve filter
    ShaderReadInfo resolve_shader_info[] = {
        {std::string("Assets/Shaders/screenquad.vert"), ShaderType::VERTEX_SHADER},
        {std::string("Assets/Shaders/msaa_resolve.frag"), ShaderType::FRAGMENT_SHADER}};
    program = shaderMgr->ReadShaders(resolve_shader_info, 2);
    pass = std::make_shared<RenderPass>(program);

    _msaa_resolve_mat = std::make_shared<Material>();
    _msaa_resolve_mat->AddRenderPass(pass);
    _msaa_resolve_mat->SetMainTexture(_rt_msaa->GetTexture());
}

void AntiAliasingScene::UpdateScene()
{
    // Camera::Ptr camera = _current_aa_option == AAOption::MSAA_SW ?
    //     _msaa_camera : _maincamera;
    Camera::Ptr camera = _maincamera;
    const Viewport& viewport = camera->GetViewport();

    _viewport_params = glm::vec4((float32)(viewport.Width()), (float32)(viewport.Height()), viewport.AspectRatio(), 1.0f);
    _view_mat = camera->GetViewMatrix();
    _projection_mat = camera->GetProjectionMatrix();
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
        tint_color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
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
        tint_color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
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
        _maincamera->ClearRenderContext();
        glEnable(GL_MULTISAMPLE);
        RenderGeometrys();
        glDisable(GL_MULTISAMPLE);
        break;
    case AAOption::MSAA_SW:
        _rt_msaa->Bind();
        {
            // render with msaa fbo
            // _msaa_camera->ClearRenderContext();
            _maincamera->ClearRenderContext();
            if(_enable_multisample)
                glEnable(GL_MULTISAMPLE);
            else
                glDisable(GL_MULTISAMPLE);
            RenderGeometrys();
            if(_enable_multisample)
                glDisable(GL_MULTISAMPLE);

            // blit to backbuffer
            if(_current_msaa_sw_option == MSAASWOption::MSAA_SW_BACKBUFFER)
            {
                _rt_msaa->BlitColorToBackBuffer(1024, 768);
            }
            // blit to screen fbo
            else if(_current_msaa_sw_option == MSAASWOption::MSAA_SW_SCREENFBO)
            {
                _rt_msaa->BlitColor(_rt_msaa_resolve);
            }
        }
        _rt_msaa->UnBind();

        // render screen quad
        if(_current_msaa_sw_option == MSAASWOption::MSAA_SW_SCREENFBO || 
           _current_msaa_sw_option == MSAASWOption::MSAA_SW_RESOLVE)
        {
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

void AntiAliasingScene::RenderGeometrys()
{
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

void AntiAliasingScene::RenderScreenQuad()
{
    RenderGeometry(_screen_quad_msaa, 9);
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
