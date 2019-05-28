
#include <iostream>

#include "twShader.h"

#include "twFirstTriangle.h"

namespace TwinkleGraphics
{
FirstTriangle::FirstTriangle(std::string& name)
    : GLPlugin(name)
{
}

FirstTriangle::~FirstTriangle()
{
}

void FirstTriangle::Install()
{
    std::cout << "Plugin Install:"<< _name << ".\n";

    Viewport viewport(Rect(512, 0, 512, 768), 17664U, RGBA(1.0f, 1.f, 0.f, 1.f));
    TriangleView* view = new TriangleView(viewport);
    view->Initialize();
    this->AddView(view);

    ShaderLoadInfo shaders_info[] = {
        { ShaderType::VERTEX_SHADER, "Assets/Shaders/firstTriangle.vert" },
        { ShaderType::FRAGMENT_SHADER, "Assets/Shaders/firstTriangle.frag" },
    };

    ShaderManagerInst shaderMgr;
    ShaderProgram::Ptr shader_program = shaderMgr->ReadShaders(shaders_info, 2);

    std::cout << "shader_program use count:" << shader_program.use_count() << std::endl;
}

void FirstTriangle::UnInstall()
{
    std::cout << "Plugin UnInstall:" << _name << ".\n";
}



void TriangleView::RenderImplement()
{
    //std::cout << "RenderImplement: FirstTriangle." << std::endl;
}

void TriangleView::Initialize()
{
    //create vao
    _vaos = new uint32[1];
    glGenVertexArrays(1, _vaos);
    glBindVertexArray(_vaos[0]);

    //create triangle vertices
    float32 vertices[3][3] = {
        {-1.0f, -1.0f, 0.0f},
        {1.0f, -1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f}
    };

    //create elements(indices)
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

    //initialize camera
}

void TriangleView::Destroy()
{
    //Delete buffers
}

} // namespace TwinkleGraphics
