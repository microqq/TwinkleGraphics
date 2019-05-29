
#include <iostream>

#include "twShader.h"

#include "twFirstTriangle.h"

namespace TwinkleGraphics
{
FirstTriangle::FirstTriangle(std::string& name)
    : GLPlugin(name)
    , _view(nullptr)
{
}

FirstTriangle::~FirstTriangle()
{
}

void FirstTriangle::Install()
{
    Plugin::Install();

    // Initilize view
    Viewport viewport(Rect(512, 0, 512, 768), 17664U, RGBA(1.0f, 1.f, 0.f, 1.f));
    _view = new TriangleView(viewport);
    _view->Initialize();

    _views[_views_count++] = _view;
}

void FirstTriangle::UnInstall()
{
    _view->Destroy();
    SAFE_DEL(_view);

    Plugin::UnInstall();
}



void TriangleView::Initialize()
{
    //create vao
    _vaos = new uint32[1];
    glGenVertexArrays(1, _vaos);
    glBindVertexArray(_vaos[0]);

    //create triangle vertices & elements(indices)
    float32 vertices[3][3] = {
        {-1.0f, -1.0f, 0.0f},
        {1.0f, -1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f}
    };
    uint32 elements[3] = {
        0, 1, 2
    };

    //create vbo & ebo
    _vbos = new uint32[1];
    glGenBuffers(1, _vbos);
    _ebos = new uint32[1];
    glGenBuffers(1, _ebos);

    //bind vertex array buffer, bind buffer data
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, 36, vertices, GL_DYNAMIC_DRAW);

    //bind element array buffer, bind buffer data 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebos[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12, elements, GL_DYNAMIC_DRAW);    

    //vertex attribute layout


    //create shader
    ShaderReadInfo shaders_info[] = {
        {std::string("Assets/Shaders/first_triangle.vert"), ShaderType::VERTEX_SHADER},
        {std::string("Assets/Shaders/first_triangle.frag"), ShaderType::FRAGMENT_SHADER}};

    ShaderManagerInst shaderMgr;
    ShaderProgram::Ptr shader_program = shaderMgr->ReadShaders(shaders_info, 2);

    //initialize camera

}

void TriangleView::RenderImplement()
{
    //std::cout << "RenderImplement: FirstTriangle." << std::endl;
}

void TriangleView::Destroy()
{
    //Delete buffers
}

} // namespace TwinkleGraphics
