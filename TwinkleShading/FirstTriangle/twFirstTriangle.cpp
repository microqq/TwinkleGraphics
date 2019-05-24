
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

    InstallDefaultView();

    Viewport viewport(Rect(512, 0, 512, 768), 17664U, RGBA(1.0f, 1.f, 0.f, 1.f));
    std::cout << viewport.rect.z << "\n";
    _views[_views_count++] = new View(viewport);
}

void FirstTriangle::UnInstall()
{
    std::cout << "Plugin UnInstall:" << _name << ".\n";
}

} // namespace TwinkleGraphics
