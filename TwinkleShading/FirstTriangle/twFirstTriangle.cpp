
#include <iostream>

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
    _views[_views_count++] = new TriangleView(viewport);
}

void FirstTriangle::UnInstall()
{
    std::cout << "Plugin UnInstall:" << _name << ".\n";
}



void TriangleView::RenderImplement()
{
    std::cout << "Render FirstTriangle." << std::endl;
}

void TriangleView::Initialize()
{
    //create vao
    uint32* vaos = new uint32[1];
    glGenVertexArrays(1, vaos);
    glBindVertexArrays(vaos[0]);

    //create triangle vertices
    float32 vertices[3][3] = {
        {-1.0f, -1.0f, 0.0f},
        {1.0f, -1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f}
    };

    uint32 elements[3] = {
        0, 1, 2
    };

    //create vbo & ido
    uint32* vbos = new uint32[1];
    glGenBuffers(1, vbos);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, 36, vertices, GL_DYNAMIC_DRAW);

    uint32* ibos = new uint32[1];
    glGenBuffers(1, ibos);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibos[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12, elements, GL_DYNAMIC_DRAW);

        
    //initialize camera
}

} // namespace TwinkleGraphics
