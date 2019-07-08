
#include <iostream>

#include "imgui.h"

#include "twBasicGeometry.h"

namespace TwinkleGraphics
{
BasicGeometry::BasicGeometry(std::string& name)
    : GLPlugin(name)
    , _view(nullptr)
{
}

BasicGeometry::~BasicGeometry()
{
    SAFE_DEL(_view);
}

void BasicGeometry::Install()
{
    Plugin::Install();

    // Initilize view
    Viewport viewport(Rect(0, 0, 800, 640), 17664U, RGBA(0.0f, 0.f, 0.f, 1.f));
    Camera::Ptr camera = std::make_shared<Camera>(viewport, 45.0f, 0.1f, 1000.0f);
    _view = new BasicGeometryView();
    _view->SetViewCamera(camera);
    _view->Initialize();

    _views[_views_count++] = _view;
}

void BasicGeometry::UnInstall()
{
    _view->Destroy();
    SAFE_DEL(_view);

    Plugin::UnInstall();
}



void BasicGeometryView::Initialize()
{
    //create vertex buffer object
    _vbos = new uint32[7];
    glGenBuffers(7, _vbos);
    _ebos = new uint32[7];
    glGenBuffers(7, _ebos);

    //create vertex array object
    _vaos = new uint32[7];
    glGenVertexArrays(7, _vaos);

    //create sphere
    CreateUVSphere();
    CreateNorCubeSphere();
    CreateIcoSphere();
    CreateQuad();
    CreateCube();
    CreateInfinitePlane();

    _orbitcontrol = new OrbitControl(_camera);

    //camera view setting: frustum and its position, orientation
    _camera->Translate(glm::vec3(0.0f, 5.0f, 50.0f));
    _view_mat = glm::mat4(_camera->GetViewMatrix());
    _projection_mat = glm::mat4(_camera->GetProjectionMatrix());

    _model_mat = glm::mat4(1.0f);

    _mvp_mat = _projection_mat * _view_mat;

    //model transformation
    // _model_mat = glm::rotate(_model_mat, glm::radians<float32>(-90), glm::vec3(1.0f, 0.0f, 0.0f));
    _model_mat = glm::scale(_model_mat, glm::vec3(0.5f, 0.5f, 0.5f));

    ShaderManagerInst shaderMgr;

    //create basic-geometry shader
    ShaderReadInfo shaders_info[] = {
        {std::string("Assets/Shaders/basic_geometry.vert"), ShaderType::VERTEX_SHADER},
        {std::string("Assets/Shaders/basic_geometry.frag"), ShaderType::FRAGMENT_SHADER}};

    _program = shaderMgr->ReadShaders(shaders_info, 2);
    {
        ShaderProgramUse use_program(_program);
        //get shader uniform location
        _model_mat_loc = glGetUniformLocation(_program->GetRes().id, "model");
        _view_mat_loc = glGetUniformLocation(_program->GetRes().id, "view");
        _projection_mat_loc = glGetUniformLocation(_program->GetRes().id, "projection");
    }


    _plane_param = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    //create infinite-plane shader
    ShaderReadInfo infplane_shaders_info[] = {
        {std::string("Assets/Shaders/infinite_plane.vert"), ShaderType::VERTEX_SHADER},
        {std::string("Assets/Shaders/infinite_plane.frag"), ShaderType::FRAGMENT_SHADER}};

    _infinite_plane_program = shaderMgr->ReadShaders(infplane_shaders_info, 2);
    {
        ShaderProgramUse use_program(_infinite_plane_program);
        _infplane_mvp_loc = glGetUniformLocation(_infinite_plane_program->GetRes().id, "mvp");
        _planeparam_loc = glGetUniformLocation(_infinite_plane_program->GetRes().id, "plane_param");
    }


    const Viewport& viewport = _camera->GetViewport();
    _viewport_params = glm::vec4((float32)(viewport.Width()), (float32)(viewport.Height()), viewport.AspectRatio(), 1.0f);
    _line_params = glm::vec4(0.05f, 0.01f, 0.99f, 1.0f);
    _line_color = glm::vec3(1.0f, 1.0f, 1.0f);
    _line_points = new glm::vec3[5]{
        glm::vec3(-5.f, 2.5f, 0.0f),
        glm::vec3(0.0f, -5.0f, 0.0f),
        glm::vec3(-3.f, 2.0f, 0.0f),
        glm::vec3(5.f, 2.5f, 0.0f),
        glm::vec3(-5.f, -2.5f, -5.0f)
    };
    CreateLine();

    ShaderReadInfo line_shader_info[] = {
        {std::string("Assets/Shaders/line.vert"), ShaderType::VERTEX_SHADER},
        {std::string("Assets/Shaders/line.geom"), ShaderType::GEOMETRY_SHADER},
        {std::string("Assets/Shaders/line.frag"), ShaderType::FRAGMENT_SHADER}
    };
    _line_program = shaderMgr->ReadShaders(line_shader_info, 3);
    {
        ShaderProgramUse use_program(_line_program);
        _line_mvp_loc = glGetUniformLocation(_line_program->GetRes().id, "mvp");
        _line_parameters_loc = glGetUniformLocation(_line_program->GetRes().id, "line_params");
        _viewport_loc = glGetUniformLocation(_line_program->GetRes().id, "viewport_params");
        _line_color_loc = glGetUniformLocation(_line_program->GetRes().id, "line_color");
    }
}

void BasicGeometryView::Advance(float64 delta_time)
{
    View::Advance(delta_time);

    const Viewport& viewport = _camera->GetViewport();
    _viewport_params = glm::vec4((float32)(viewport.Width()), (float32)(viewport.Height()), viewport.AspectRatio(), 1.0f);
    _projection_mat = _camera->GetProjectionMatrix();
    //_projection_mat = glm::perspective(glm::radians(45.0f), viewport.AspectRatio(), 0.1f, 1000.0f);
}

void BasicGeometryView::RenderImpl()
{
    // RenderUVSphere();

    // RenderNorCubeSphere();

    // RenderIcoSphere();

    // RenderQuad();

    // RenderCube();

    if(_current_mesh != nullptr)
    {
        if( _current_mesh_index != -1 &&
            _current_mesh_index != 6 &&
            _current_mesh_index != 5)
        {
            if(_display_infplane)
            {
                RenderInfinitePlane();
            }

            RenderGeometry(_current_mesh, _current_mesh_index, _front_face);
        }
        else if(_current_mesh_index == 6)
        {
            RenderInfinitePlane();
        }
        else if(_current_mesh_index == 5)
        {
            if(_display_infplane)
            {
                RenderInfinitePlane();
            }
            RenderLine();
        }
    }
}

void BasicGeometryView::OnGUI()
{
    // ImGui::Begin(u8"相机参数");
    // {
    //     bool changed = ImGui::InputFloat3(u8"相机位置", glm::value_ptr(_eye));
    //     changed |= ImGui::InputFloat3(u8"视线中心", glm::value_ptr(_center));
    //     changed |= ImGui::InputFloat3(u8"向上向量", glm::value_ptr(_up));

    //     if(changed)
    //     {
    //         _view_mat = glm::lookAtRH(_eye, _center, _up);
    //         _mvp_mat = _projection_mat * _view_mat;
    //     }
    // }
    // ImGui::End();

    ImGui::Begin(u8"创建几何形体");
    {
        if(ImGui::RadioButton(u8"球体(UV Sphere)", &_current_mesh_index, 0))
        {
            if(_uvsphere == nullptr)
            {
                CreateUVSphere();
            }
            _front_face = GL_CW;
            _current_mesh = _uvsphere;
        }
        if(ImGui::RadioButton(u8"球体(Nor Cube Sphere)", &_current_mesh_index, 1))
        {
            if(_norcubesphere == nullptr)
            {
                CreateNorCubeSphere();
            }
            _front_face = GL_CCW;
            _current_mesh = _norcubesphere;
        }
        if(ImGui::RadioButton(u8"球体(Ico Sphere)", &_current_mesh_index, 2))
        {
            if(_icosphere == nullptr)
            {
                CreateIcoSphere();
            }
            _front_face = GL_CCW;
            _current_mesh = _icosphere;
        }
        if(ImGui::RadioButton(u8"四边形", &_current_mesh_index, 4))
        {
            if(_quad == nullptr)
            {
                CreateQuad();
            }
            _front_face = GL_CCW;
            _current_mesh = _quad;
        }
        if(ImGui::RadioButton(u8"立方体", &_current_mesh_index, 3))
        {
            if(_cube == nullptr)
            {
                CreateCube();
            }
            _front_face = GL_CCW;
            _current_mesh = _cube;
        }
        if(ImGui::RadioButton(u8"线", &_current_mesh_index, 5))
        {
            if(_line == nullptr)
            {
                CreateLine();
            }
            _front_face = GL_CCW;
            _current_mesh = _line;
        }
        if(ImGui::RadioButton(u8"无限平面", &_current_mesh_index, 6))
        {
            if(_infinite_plane == nullptr)
            {
                CreateInfinitePlane();
            }
            _front_face = GL_CCW;
            _current_mesh = _infinite_plane;
        }
    }
    ImGui::End();

    OnParametersGUI();
}

void BasicGeometryView::OnParametersGUI()
{
    if(_current_mesh_index != -1)
    {
        ImGui::Begin(u8"参数设置");
        ImGui::Checkbox(u8"剔除背面", &_cull_face);
        ImGui::Checkbox(u8"线框模式", &_polygon_linemode);
        ImGui::Checkbox(u8"显示平面", &_display_infplane);

        if(_current_mesh_index == 5)
        {
            if(_current_mesh == _line)
            {
                ImGui::InputFloat(u8"线宽", &(_line_params.x), 0.01f);
                ImGui::Checkbox(u8"抗锯齿", &_line_antialiasing);
                {
                    if(_line_antialiasing)
                    {
                        ImGui::InputFloat(u8"羽化", &(_line_params.y), 0.001f);
                        _line_params.w = 1.0f;
                    }
                    else
                    {
                        _line_params.w = 0.0f;
                    }
                }
                ImGui::InputFloat(u8"连接点限制参数", &(_line_params.z), 0.001f);
                ImGui::Checkbox(u8"虚线", &_line_isdashed);
                {

                }
                ImGui::Checkbox(u8"圆角直线", &_line_type);
                {

                }

                ImGui::InputFloat3(u8"颜色", glm::value_ptr(_line_color));
            }
        }
        else if(_current_mesh_index == 0)
        {
            bool recreate = false;
            recreate = ImGui::InputFloat(u8"球半径", &_sphere_radius, 0.5f, 0.0f);
            recreate |= ImGui::InputInt(u8"经线数", &_uvsphere_longitude, 1, 0);
            recreate |= ImGui::InputInt(u8"纬线数", &_uvsphere_latitude, 1, 0);

            if(recreate)
            {
                CreateUVSphere();
                _current_mesh = _uvsphere;
            }
        }
        else if(_current_mesh_index == 1 ||
            _current_mesh_index == 2)
        {
            bool recreate = false;
            recreate = ImGui::InputFloat(u8"球半径", &_sphere_radius, 0.5f, 0.0f);
            recreate |= ImGui::InputInt(u8"细分次数", &_sphere_subdivide, 1, 0);

            if(recreate)
            {
                if(_current_mesh_index == 1)
                {
                    CreateNorCubeSphere();
                    _current_mesh = _norcubesphere;
                }
                else if(_current_mesh_index == 2)
                {
                    CreateIcoSphere();
                    _current_mesh = _icosphere;
                }
            }
        }
        ImGui::End();
    }
}

void BasicGeometryView::Destroy()
{
    //unbind program
    glUseProgram(0);

    //unbind vao/ebo
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //Delete buffers
    glDeleteVertexArrays(7, _vaos);
    glDeleteBuffers(7, _vbos);
    glDeleteBuffers(7, _ebos);

    SAFE_DEL_ARR(_line_points);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void BasicGeometryView::CreateGeometry(SubMesh::Ptr submesh, uint32 index)
{
    //bind element array buffer, bind buffer data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebos[index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, submesh->GetIndiceNum() * 4, submesh->GetIndice(), GL_DYNAMIC_DRAW);

    //bind vertex array object
    glBindVertexArray(_vaos[index]);

    //bind vertex array buffer, bind buffer data
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[index]);

    glBufferData(GL_ARRAY_BUFFER, submesh->GetVerticeNum() * 12, submesh->GetVerticePos(), GL_DYNAMIC_DRAW);
    //vertex attribute layout setting
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
}

void BasicGeometryView::RenderGeometry(Mesh::Ptr mesh, int32 index, GLenum front_face)
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glFrontFace(front_face);
    if(_polygon_linemode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if(_cull_face)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
    

    //bind shader program
    ShaderProgramUse use_program(_program);

    //shader uniform setting
    glUniformMatrix4fv(_model_mat_loc, 1, GL_FALSE, glm::value_ptr(_model_mat));
    glUniformMatrix4fv(_view_mat_loc, 1, GL_FALSE, glm::value_ptr(_view_mat));
    glUniformMatrix4fv(_projection_mat_loc, 1, GL_FALSE, glm::value_ptr(_projection_mat));

    SubMesh::Ptr submesh = mesh->GetSubMesh(0);

    //draw command use vertex array object
    glBindVertexArray(_vaos[index]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebos[index]);
    glDrawElements(GL_TRIANGLES, submesh->GetIndiceNum(), GL_UNSIGNED_INT, NULL);
    // glDrawElements(GL_POINTS, submesh->GetIndiceNum(), GL_UNSIGNED_INT, NULL);


    glFrontFace(GL_CCW);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/**
 * @brief 
 * 
 */
void BasicGeometryView::RenderInfinitePlane()
{
    glEnable(GL_DEPTH_TEST);

    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    ShaderProgramUse use_program(_infinite_plane_program);
    {
        glUniformMatrix4fv(_infplane_mvp_loc, 1, GL_FALSE, glm::value_ptr(_mvp_mat));
        glUniform4fv(_planeparam_loc, 1, glm::value_ptr(_plane_param));

        SubMesh::Ptr submesh = _infinite_plane->GetSubMesh(0);

        //draw command use vertex array object
        glBindVertexArray(_vaos[6]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebos[6]);
        glDrawElements(GL_TRIANGLES, submesh->GetIndiceNum(), GL_UNSIGNED_INT, NULL);
    }
}

/**
 * @brief 直线绘制参考（感谢文章作者）：https://zhuanlan.zhihu.com/p/59541559，文中引文值得仔细阅读。
 * 另外，GPU gems 3（chapter 25：https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch25.html）有一篇关于 gpu 绘制向量图的方法介绍。
 */
void BasicGeometryView::RenderLine()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    // glBlendFunc(GL_ONE, GL_ONE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_CULL_FACE);
    // if(_polygon_linemode)
    // {
    //     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // }
    // else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    ShaderProgramUse use_program(_line_program);
    {
        glUniformMatrix4fv(_line_mvp_loc, 1, GL_FALSE, glm::value_ptr(_mvp_mat));
        glUniform4fv(_line_parameters_loc, 1, glm::value_ptr(_line_params));
        glUniform4fv(_viewport_loc, 1, glm::value_ptr(_viewport_params));
        glUniform3fv(_line_color_loc, 1, glm::value_ptr(_line_color));

        SubMesh::Ptr submesh = _line->GetSubMesh(0);

        //draw command use vertex array object
        glBindVertexArray(_vaos[5]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebos[5]);
        glDrawElements(GL_LINES_ADJACENCY, submesh->GetIndiceNum(), GL_UNSIGNED_INT, NULL);
    }

    glDisable(GL_BLEND);
}


/**
 * @brief Render UV Sphere
 * 
 */
void BasicGeometryView::RenderUVSphere()
{
    RenderGeometry(_uvsphere, 0, GL_CW);
}

void BasicGeometryView::CreateUVSphere() 
{
    _uvsphere = Mesh::CreateSphereMeshStandard(_sphere_radius, _uvsphere_longitude, _uvsphere_latitude);
    SubMesh::Ptr submesh = _uvsphere->GetSubMesh(0);

    CreateGeometry(submesh, 0);
}


/**
 * @brief Render Normalized Cube Subdivide Sphere
 * 
 */
void BasicGeometryView::RenderNorCubeSphere()
{
    RenderGeometry(_norcubesphere, 1, GL_CCW);
}

void BasicGeometryView::CreateNorCubeSphere() 
{
    _norcubesphere = Mesh::CreateSphereMeshNormalizedCube(_sphere_radius, _sphere_subdivide);
    SubMesh::Ptr submesh = _norcubesphere->GetSubMesh(0);

    CreateGeometry(submesh, 1);
}

void BasicGeometryView::RenderIcoSphere()
{
    RenderGeometry(_icosphere, 2, GL_CCW);
}

void BasicGeometryView::CreateIcoSphere() 
{
    _icosphere = Mesh::CreateSphereMeshIcosahedron(_sphere_radius, _sphere_subdivide);
    SubMesh::Ptr submesh = _icosphere->GetSubMesh(0);

    CreateGeometry(submesh, 2);
}

void BasicGeometryView::RenderCube()
{
    RenderGeometry(_cube, 3, GL_CCW);
}

void BasicGeometryView::CreateCube() 
{
    _cube = Mesh::CreateCubeMesh(5.0f);
    SubMesh::Ptr submesh = _cube->GetSubMesh(0);
    CreateGeometry(submesh, 3);
}

void BasicGeometryView::RenderQuad()
{
    RenderGeometry(_quad, 4, GL_CCW);
}

void BasicGeometryView::CreateQuad() 
{
    _quad = Mesh::CreateQuadMesh(5.0f, 5.0f);
    SubMesh::Ptr submesh = _quad->GetSubMesh(0);
    CreateGeometry(submesh, 4);
}

void BasicGeometryView::CreateLine()
{
    _line = Mesh::CreateLineMesh(_line_points, 5);
    SubMesh::Ptr SubMesh = _line->GetSubMesh(0);
    CreateGeometry(SubMesh, 5);
}

void BasicGeometryView::CreateInfinitePlane()
{
    _infinite_plane = Mesh::CreateQuadMesh(2.0f, 2.0f);
    SubMesh::Ptr submesh = _infinite_plane->GetSubMesh(0);
    CreateGeometry(submesh, 6);
}

} // namespace TwinkleGraphics
