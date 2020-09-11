

#include "imgui.h"

#include "twAntiAliasing.h"
#include "twImage.h"
#include "twRenderTexture.h"
#include "twMaterialInstance.h"
#include "twImageManager.h"
#include "twModelManager.h"

#include "twConsoleLog.h"

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
    Camera::Ptr camera = std::make_shared<Camera>(viewport, 45.0f, 0.1f, 10000.0f);
    _view->SetViewCamera(camera);

    FirstPersonControl::Ptr cameraControl = std::make_shared<FirstPersonControl>(camera);
    cameraControl->SetMinDistance(-5000.0f);
    cameraControl->SetMaxDistance(5000.0f);
    cameraControl->SetStep(2.0f, 2.0f);
    _view->SetCameraControl(cameraControl);

    AntiAliasingScene::Ptr scene = std::make_shared<AntiAliasingScene>();
    _view->AttachScene(scene);

    _views[_viewsCount++] = _view;
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
    TwinkleGraphics::MainMenuBar();

    ImGui::Begin(u8"反走样");
    {
        AntiAliasingScene* scene = dynamic_cast<AntiAliasingScene*>(_scene.get());
        int32& current_aa_option = scene->GetCurrentAAOption();
        int32& current_msaa_sw_option = scene->_currentMSAASWOption;
        int32& current_rsf_option = scene->_resolveFitlerOption;

        ImGui::RadioButton(u8"NO AA", &(current_aa_option), AAOption::NONE);
        ImGui::RadioButton(u8"MSAA_HW", &(current_aa_option), AAOption::MSAA_HW);
        ImGui::RadioButton(u8"MSAA_SW", &current_aa_option, AAOption::MSAA_SW);
        if(current_aa_option == AAOption::MSAA_SW)
        {
            ImGui::BeginGroup();
            ImGui::Indent();
            ImGui::Checkbox(u8"多重采样", &(scene->_enableMultisample));
            ImGui::RadioButton(u8"Back Buffer", &(current_msaa_sw_option), MSAASWOption::MSAA_SW_BACKBUFFER);
            ImGui::RadioButton(u8"Screen FBO", &(current_msaa_sw_option), MSAASWOption::MSAA_SW_SCREENFBO);
            ImGui::RadioButton(u8"Resolve Filter", &(current_msaa_sw_option), MSAASWOption::MSAA_SW_RESOLVE);
            if(current_msaa_sw_option == MSAASWOption::MSAA_SW_RESOLVE)
            {
                ImGui::Indent();
                ImGui::Combo("", &current_rsf_option, 
                    "Box\0Tiangle\0Cubic\0Hann and Hamming Window\0Blackman Window\0Kaiser Window\0"
                    "Lanczos3 Window\0Lanczos4 Window\0Lanczos6 Window\0\0"
                );
                ImGui::Unindent();

                if(current_rsf_option >= ResolveFilterOption::BOX)
                {
                    Material::Ptr resolve_mat = scene->_msaaResolveMat;
                    resolve_mat->SetSimpleUniformValue<int, 1>("filterOption", current_rsf_option);
                    // set resolve filter option
                }

                MeshRenderer::Ptr renderer = scene->_screenQuadMSAA->GetMeshRenderer();
                renderer->SetSharedMaterial(scene->_msaaResolveMat);
            }
            else
            {
                MeshRenderer::Ptr renderer = scene->_screenQuadMSAA->GetMeshRenderer();
                renderer->SetSharedMaterial(scene->_screenQuadMat);
            }
            ImGui::EndGroup();
        }

        ImGui::RadioButton(u8"SSAA", &current_aa_option, AAOption::SSAA);
        ImGui::RadioButton(u8"CSAA", &current_aa_option, AAOption::CSAA);
        ImGui::RadioButton(u8"CFAA", &current_aa_option, AAOption::CFAA);
        ImGui::RadioButton(u8"FXAA", &current_aa_option, AAOption::FXAA);
    }

    ImGui::End();

    Select3DModel();
}

void AntiAliasingView::Select3DModel()
{
    ImGui::Begin(u8"选择模型");
    FileDialogPanel(".obj,.OBJ,.fbx,.FBX,.3ds,.3DS,.gsm,.GSM,.lwo,.LWO,.meshdata,.MESHDATA,.geo,.GEO", _selectFileInfo);
    if(_selectFileInfo.selectChanged)
    {
        AntiAliasingScene *scene = dynamic_cast<AntiAliasingScene *>(_scene.get());
        scene->Load3DModel(_selectFileInfo.filePathName);

        _selectFileInfo.selectChanged = false;
    }
    ImGui::End();

    // std::cout << _selectFileInfo.filePathName << std::endl;
    // std::cout << _selectFileInfo.filePath << std::endl;
    // std::cout << _selectFileInfo.fileName << std::endl;
}

void AntiAliasingScene::CreateScene()
{
    // create conel box
    _planeLeft.reset(CreatePlane(glm::vec3(1.0f, 0.0f, 0.0f), 30.0f));
    Transform::Ptr trans = _planeLeft->GetTransform();
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

    _rootTrans = std::make_shared<Transform>();

    // icosphere
    _sphere.reset(CreateIcosahedronSphere(3.0f, 10, MeshDataFlag(MeshDataFlag::HAS_UV)));
    {
        trans = _sphere->GetTransform();
        trans->Translate(glm::vec3(-5.0f, -5.0f, 0.0f));
        trans->SetParent(_rootTrans);

        CreateGeometry(_sphere, 5);

        ImageManager& imageMgr = ImageMgrInstance();
        std::string imageFilename{"Assets/Textures/TantolundenCube.dds"};
        Image::Ptr image = imageMgr.ReadImage(imageFilename.c_str());
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
        trans->SetParent(_rootTrans);

        CreateGeometry(_cube, 6);
    }

    // triangles
    glm::vec4 tintcolor(0.6f, 0.9f, 0.2f, 1.0f);
    glm::vec3 p0(-3.0f, 5.0f, 0.0f), p1(-4.0f, 0.0f, 0.0f), p2(3.0f, 5.2f, 0.0f);
    _triangleBack.reset(CreateTriangle(p0, p1, p2));
    {
        Material::Ptr material = _triangleBack->GetMeshRenderer()->GetSharedMaterial();
        material->SetVecUniformValue<float32, 4>("tintColor", tintcolor);

        CreateGeometry(_triangleBack, 7);
    }
    p0 = glm::vec3(-2.0f, 3.0f, 0.0f);
    p1 = glm::vec3(-3.0f, 0.0f, 0.0f);
    p2 = glm::vec3(3.0f, 2.3f, 0.0f);
    _triangle_front.reset(CreateTriangle(p0, p1, p2));
    {
        tintcolor = glm::vec4(1.0f, 0.3f, 0.8f, 1.0f);
        Material::Ptr material = _triangle_front->GetMeshRenderer()->GetSharedMaterial();
        material->SetVecUniformValue<float32, 4>("tintColor", tintcolor);

        CreateGeometry(_triangle_front, 8);
    }

    // rt for msaa resolve fitler 
    _rtMSAA = std::make_shared<RenderTexture>(1024, 768, GL_RGBA8
        , GL_RGBA, true, false, true, _numSamples, true);
    _rtMSAA->Create(nullptr);

    _rtScreen = std::make_shared<RenderTexture>(1024, 768, GL_RGBA8
        , GL_RGBA, false, false, false, 1, false);
    _rtScreen->Create(nullptr);
    _rtScreen->GetTexture()->SetFilter<FilterParam::MIN_FILTER>(FilterMode::LINEAR);
    _rtScreen->GetTexture()->SetFilter<FilterParam::MAG_FILTER>(FilterMode::LINEAR);

    vec2 size(2, 2);
    _screenQuadMSAA = std::make_shared<Quad>(size, MeshDataFlag(8));
    // _screenQuadMSAA->GenerateMeshInternal();
    {
        CreateGeometry(_screenQuadMSAA, 9);
    }

    // create screen quad material
    char* vertMacros[1] = { const_cast<char*>(VertexLayoutDefines[3]) };
    ShaderManager& shaderMgr = ShaderMgrInstance();

    /**
     * 
     * Debug Mode With GDB:
     *  Call ReadShaderAsync in plugin Antialiasing DLL, call future.get() which
     *  will throw exception with unknown/just-in-time compiled code.
     * 
     * So, fix this problem with solution that makes the operations of _futures.push & future.get() in common dll.
     * 
     *  but call ReadShaderAsync in Commmon dll or executable, it works fine. 
     */
    ShaderOption option(
        ShaderOption::OptionData{std::string("Assets/Shaders/cube.frag"), ShaderType::FRAGMENT_SHADER});
    shaderMgr.ReadShaderAsync("Assets/Shaders/cube.frag", &option);

    ShaderOption shader_info[] = {
        ShaderOption::OptionData{std::string("Assets/Shaders/screenquad.vert"), ShaderType::VERTEX_SHADER, 1, vertMacros},
        ShaderOption::OptionData{std::string("Assets/Shaders/screenquad.frag"), ShaderType::FRAGMENT_SHADER}};
    ShaderProgram::Ptr program = shaderMgr.ReadShaders(shader_info, 2);
    RenderPass::Ptr pass = std::make_shared<RenderPass>(program);

    _screenQuadMat = std::make_shared<Material>();
    _screenQuadMat->AddRenderPass(pass);

    _screenQuadMat->SetMainTexture(_rtScreen->GetTexture());
    MeshRenderer::Ptr quad_renderer = std::make_shared<MeshRenderer>();
    quad_renderer->SetSharedMaterial(_screenQuadMat);
    quad_renderer->SetMesh(_screenQuadMSAA->GetMesh());
    _screenQuadMSAA->SetMeshRenderer(quad_renderer);


    // create material for custom msaa resolve filter
    ShaderOption resolve_shader_info[] = {
        ShaderOption::OptionData{std::string("Assets/Shaders/screenquad.vert"), ShaderType::VERTEX_SHADER, 1, vertMacros},
        ShaderOption::OptionData{std::string("Assets/Shaders/msaaResolve.frag"), ShaderType::FRAGMENT_SHADER}};
    program = shaderMgr.ReadShaders(resolve_shader_info, 2);
    pass = std::make_shared<RenderPass>(program);

    _msaaResolveMat = std::make_shared<Material>();
    _msaaResolveMat->AddRenderPass(pass);
    _msaaResolveMat->SetMainTexture(_rtMSAA->GetTexture());
    _msaaResolveMat->SetSimpleUniformValue<int, 1>("samplecount", _numSamples);

    CreateSkybox();
    // CreateInfinitePlane();
}

void AntiAliasingScene::Load3DModel(std::string filename)
{
    ModelManager& modelMgr = ModelMgrInstance();
    _model = modelMgr.ReadModel(filename.c_str());

    // _model = modelMgr->ReadModel("Assets/Models/Sponza/sponza.obj");
    // _sponza = modelMgr->ReadModel("Assets/Models/bunny.obj");
    // _sponza = modelMgr->ReadModel("Assets/Models/Armadillo/Armadillo.ply");
}

void AntiAliasingScene::UpdateScene()
{
    // Camera::Ptr camera = _current_aa_option == AAOption::MSAA_SW ?
    //     _msaa_camera : _maincamera;
    Camera::Ptr camera = _maincamera;
    const Viewport& viewport = camera->GetViewport();

    _viewportParams = glm::vec4((float32)(viewport.Width()), (float32)(viewport.Height()), viewport.AspectRatio(), 1.0f);
    _viewMat = camera->GetViewMatrix();
    _projectionMat = camera->GetProjectionMatrix();
    _mvpMat = _projectionMat * _viewMat;

    Material::Ptr skyboxmat = _skybox->GetMeshRenderer()->GetMaterial();
    mat4 rotate_mat = glm::mat4_cast(camera->GetOrientation());
    // mat4 mvp = _projection_mat * glm::mat4(glm::mat3(_view_mat));
    mat4 mvp = _projectionMat * rotate_mat;
    skyboxmat->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);

    // Material::Ptr infplaneMaterial = _infinitePlane->GetMeshRenderer()->GetSharedMaterial();
    // infplaneMaterial->SetMatrixUniformValue<float32, 4, 4>("mvp", _mvpMat);

    Material::Ptr material_plane_left = _planeLeft->GetMeshRenderer()->GetSharedMaterial();
    glm::mat4 model = _planeLeft->GetTransform()->GetLocalToWorldMatrix();
    mvp = _mvpMat * model;
    material_plane_left->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
    glm::vec4 tintColor(0.7f, 0.0f, 0.0f, 1.0f);
    material_plane_left->SetVecUniformValue<float32, 4>("tintColor", tintColor);

    Material::Ptr material_plane_top = _planeTop->GetMeshRenderer()->GetSharedMaterial();
    {
        model = _planeTop->GetTransform()->GetLocalToWorldMatrix();
        mvp = _mvpMat * model;
        material_plane_top->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
        tintColor = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
        material_plane_top->SetVecUniformValue<float32, 4>("tintColor", tintColor);
    }

    Material::Ptr material_plane_right = _planeRight->GetMeshRenderer()->GetSharedMaterial();
    {
        model = _planeRight->GetTransform()->GetLocalToWorldMatrix();
        mvp = _mvpMat * model;
        material_plane_right->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
        tintColor = glm::vec4(0.0f, 0.7f, 0.0f, 1.0f);
        material_plane_right->SetVecUniformValue<float32, 4>("tintColor", tintColor);
    }

    Material::Ptr material_plane_bottom = _planeBottom->GetMeshRenderer()->GetSharedMaterial();
    {
        model = _planeBottom->GetTransform()->GetLocalToWorldMatrix();
        mvp = _mvpMat * model;
        material_plane_bottom->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
        tintColor = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
        material_plane_bottom->SetVecUniformValue<float32, 4>("tintColor", tintColor);
    }

    Material::Ptr material_plane_back = _planeBack->GetMeshRenderer()->GetSharedMaterial();
    {
        model = _planeBack->GetTransform()->GetLocalToWorldMatrix();
        tintColor = glm::vec4(0.7f, 0.7f, 0.0f, 1.0f);
        mvp = _mvpMat * model;
        material_plane_back->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
        material_plane_back->SetVecUniformValue<float32, 4>("tintColor", tintColor);
    }

    Material::Ptr material_sphere = _sphere->GetMeshRenderer()->GetSharedMaterial();
    {
        Transform::Ptr trans = _sphere->GetTransform();
        trans->Rotate(0.002f, glm::vec3(0.0f, 1.0f, 0.0f));
        mvp = _mvpMat * trans->GetLocalToWorldMatrix();
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

        mvp = _mvpMat * trans->GetLocalToWorldMatrix();
        material_cube->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
    }

    Material::Ptr material_triangle = _triangleBack->GetMeshRenderer()->GetSharedMaterial();
    {
        material_triangle->SetMatrixUniformValue<float32, 4, 4>("mvp", _mvpMat);
    }
    material_triangle = _triangle_front->GetMeshRenderer()->GetSharedMaterial();
    {
        material_triangle->SetMatrixUniformValue<float32, 4, 4>("mvp", _mvpMat);
    }

    _rootTrans->Rotate(0.001f, glm::vec3(0.0f, 1.0f, 0.0f));


    //update sponza -- just for test
    if(_model != nullptr)
    {
        int32 count = _model->GetGeometriesCount();
        for(int32 i = 0; i < count; i++)
        {
            Geometry::Ptr geo = _model->GetGeometry(i);
            MeshRenderer::Ptr geoRenderer = geo->GetMeshRenderer();
            if(geoRenderer == nullptr)
            {
                continue;
            }

            model = geo->GetTransform()->GetLocalToWorldMatrix();
            mvp = _mvpMat * model;

            for(int32 k = 0, num = geoRenderer->GetMaterialCount(); k < num; k++)
            {
                Material::Ptr geoMat = geoRenderer->GetMaterial(k);
                geoMat->SetMatrixUniformValue<float32, 4, 4>("mvp", mvp);
                glm::vec4 tintColor(0.6f, 0.6f, 0.6f, 1.0f);
                geoMat->SetVecUniformValue<float32, 4>("tintColor", tintColor);
            }
        }
    }
}

void AntiAliasingScene::RenderScene()
{
    switch (_currentAAOption)
    {
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
            if(_enableMultisample)
                glEnable(GL_MULTISAMPLE);
            else
                glDisable(GL_MULTISAMPLE);
            RenderGeometrys();
            if(_enableMultisample)
                glDisable(GL_MULTISAMPLE);

            // blit to backbuffer
            if(_currentMSAASWOption == MSAASWOption::MSAA_SW_BACKBUFFER)
            {
                _rtMSAA->BlitColorToBackBuffer(1024, 768);
            }
            // blit to screen fbo
            else if(_currentMSAASWOption == MSAASWOption::MSAA_SW_SCREENFBO)
            {
                _rtMSAA->BlitColor(_rtScreen);
            }
        }
        _rtMSAA->UnBind();

        // render screen quad
        if(_currentMSAASWOption == MSAASWOption::MSAA_SW_SCREENFBO || 
           _currentMSAASWOption == MSAASWOption::MSAA_SW_RESOLVE)
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
    // RenderGeometry(_triangle_front, 8);
    // glDisable(GL_POLYGON_OFFSET_FILL);

    //just for test
    if(_model != nullptr)
    {
        _model->Render();
    }
}

void AntiAliasingScene::RenderScreenQuad()
{
    RenderGeometry(_screenQuadMSAA, 9);
}

void AntiAliasingScene::RenderSkybox()
{
    RenderGeometry(_skybox, 10);
}

void AntiAliasingScene::RenderInfinitePlane()
{
    // glEnable(GL_MULTISAMPLE);
    RenderGeometry(_infinitePlane, 11);
    // glDisable(GL_MULTISAMPLE);
}


void AntiAliasingScene::ResizeScreenTexture()
{

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
    Mesh::Ptr mesh = geom->GetMesh();    
    SubMesh::Ptr submesh = mesh->GetSubMesh(0);

    //bind vertex array object
    glBindVertexArray(_vaos[index]);

    //bind element array buffer, bind buffer data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebos[index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, submesh->GetIndiceNum() * 4, submesh->GetIndice(), GL_DYNAMIC_DRAW);

    //bind vertex array buffer, bind buffer data
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[index]);
    int32 verticenum = mesh->GetVerticeNum();
    if(geom->GetMeshDataFlag() == MeshDataFlag::DEFAULT)
    {
        glBufferData(GL_ARRAY_BUFFER, mesh->GetVerticeNum() * 12, mesh->GetVerticePos(), GL_DYNAMIC_DRAW);
    }
    else
    {
        // map vertexbuffer data(position & uv)
        glBufferData(GL_ARRAY_BUFFER, mesh->GetDataSize(), nullptr, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, verticenum * 12, mesh->GetVerticePos());

        if((geom->GetMeshDataFlag() & MeshDataFlag::HAS_UV) != 0)
            glBufferSubData(GL_ARRAY_BUFFER, verticenum * 12, verticenum * 16, mesh->GetVerticeUV());
    }

    // bind vertex position
    glEnableVertexAttribArray(0);
    glBindVertexBuffer(0, _vbos[index], 0, sizeof(vec3));
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);

    // bind vertex uv
    if((geom->GetMeshDataFlag() & MeshDataFlag::HAS_UV) != 0)
    {
        glEnableVertexAttribArray(4);
        glBindVertexBuffer(1, _vbos[index], verticenum * 12, sizeof(vec4));
        glVertexAttribFormat(4, 4, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(4, 1);
    }

    glBindVertexArray(0);
}

void AntiAliasingScene::CreateSkybox()
{
    if(_skybox == nullptr)
    {
        _skybox = std::make_shared<Cube>(2.0f, MeshDataFlag::DEFAULT);
        // _skybox->GenerateMeshInternal();
        
        MeshRenderer::Ptr renderer = std::make_shared<MeshRenderer>();
        SkyboxMaterial::Ptr mat = std::make_shared<SkyboxMaterial>();

        TextureCube::Ptr cubemap = std::make_shared<TextureCube>(true);
        cubemap->SetWrap<WrapParam::WRAP_S>(WrapMode::CLAMP_TO_EDGE);
        cubemap->SetWrap<WrapParam::WRAP_T>(WrapMode::CLAMP_TO_EDGE);
        cubemap->SetWrap<WrapParam::WRAP_R>(WrapMode::CLAMP_TO_EDGE);

        cubemap->SetFilter<FilterParam::MIN_FILTER>(FilterMode::LINEAR_MIPMAP_LINEAR);
        cubemap->SetFilter<FilterParam::MAG_FILTER>(FilterMode::LINEAR);

        ImageManager& imageMgr = ImageMgrInstance();
        std::string front_info = {"Assets/Textures/skybox/front.png"};
        Image::Ptr front_image = imageMgr.ReadImage(front_info.c_str());
        std::string back_info = {"Assets/Textures/skybox/back.png"};
        Image::Ptr back_image = imageMgr.ReadImage(back_info.c_str());
        std::string left_info = {"Assets/Textures/skybox/left.png"};
        Image::Ptr left_image = imageMgr.ReadImage(left_info.c_str());
        std::string right_info = {"Assets/Textures/skybox/right.png"};
        Image::Ptr right_image = imageMgr.ReadImage(right_info.c_str());
        std::string top_info = {"Assets/Textures/skybox/top.png"};
        Image::Ptr top_image = imageMgr.ReadImage(top_info.c_str());
        std::string down_info = {"Assets/Textures/skybox/bottom.png"};
        Image::Ptr down_image = imageMgr.ReadImage(down_info.c_str());

        // ImageReadInfo front_info = {"Assets/Textures/cartoonskyes/FluffyBlueSky/Sky_Cartoon_FluffyBlueSky_Cam_0_Front+Z.png"};
        // Image::Ptr front_image = imageMgr.ReadImage(front_info);
        // ImageReadInfo back_info = {"Assets/Textures/cartoonskyes/FluffyBlueSky/Sky_Cartoon_FluffyBlueSky_Cam_1_Back-Z.png"};
        // Image::Ptr back_image = imageMgr.ReadImage(back_info);
        // ImageReadInfo left_info = {"Assets/Textures/cartoonskyes/FluffyBlueSky/Sky_Cartoon_FluffyBlueSky_Cam_3_Right-X.png"};
        // Image::Ptr left_image = imageMgr.ReadImage(left_info);
        // ImageReadInfo right_info = {"Assets/Textures/cartoonskyes/FluffyBlueSky/Sky_Cartoon_FluffyBlueSky_Cam_2_Left+X.png"};
        // Image::Ptr right_image = imageMgr.ReadImage(right_info);
        // ImageReadInfo top_info = {"Assets/Textures/cartoonskyes/FluffyBlueSky/Sky_Cartoon_FluffyBlueSky_Cam_5_Down-Y.png"};
        // Image::Ptr top_image = imageMgr.ReadImage(top_info);
        // ImageReadInfo down_info = {"Assets/Textures/cartoonskyes/FluffyBlueSky/Sky_Cartoon_FluffyBlueSky_Cam_4_Up+Y.png"};
        // Image::Ptr down_image = imageMgr.ReadImage(down_info);

        cubemap->SetPositiveX(right_image);
        cubemap->SetPositiveY(top_image);
        cubemap->SetPositiveZ(front_image);
        cubemap->SetNegativeX(left_image);
        cubemap->SetNegativeY(down_image);
        cubemap->SetNegativeZ(back_image);
        cubemap->InitStorageByOthers();

        // ImageReadInfo imageFilename = {"Assets/Textures/TantolundenCube.dds"};
        // Image::Ptr image = imageMgr.ReadImage(imageFilename);
        // cubemap->CreateFromImage(image);

        mat->SetMainTexture(cubemap);

        renderer->SetMaterial(mat);
        renderer->SetMesh(_skybox->GetMesh());

        _skybox->SetMeshRenderer(renderer);

        CreateGeometry(_skybox, 10);
    }
}

void AntiAliasingScene::CreateInfinitePlane()
{
    _infinitePlane.reset(CreateInifinitePlane(glm::vec3(0.0f, 1.0f, 0.0f), 5.0f, 2.0f, 128));

    ImageManager& imageMgr = ImageMgrInstance();
    std::string imageFilename = {"Assets/Textures/grid.png"};
    Image::Ptr image = imageMgr.ReadImage(imageFilename.c_str());

    Texture2D::Ptr texture = nullptr;
    texture = std::make_shared<Texture2D>(true, true);
    texture->CreateFromImage(image);

    texture->SetWrap<WrapParam::WRAP_S>(WrapMode::REPEAT);
    texture->SetWrap<WrapParam::WRAP_T>(WrapMode::REPEAT);

    texture->SetFilter<FilterParam::MIN_FILTER>(FilterMode::LINEAR_MIPMAP_LINEAR);
    texture->SetFilter<FilterParam::MAG_FILTER>(FilterMode::LINEAR);

    Material::Ptr material = _infinitePlane->GetMeshRenderer()->GetSharedMaterial();
    material->SetMainTexture(texture);

    CreateGeometry(_infinitePlane, 11);
}

void AntiAliasingScene::RenderGeometry(Geometry::Ptr geom, int32 index)
{
    Material::Ptr mat = geom->GetMeshRenderer()->GetMaterial();
    RenderPass::Ptr pass = mat->GetRenderPass(0);
    ShaderProgram::Ptr shader = pass->GetShaderProgram();

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
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
