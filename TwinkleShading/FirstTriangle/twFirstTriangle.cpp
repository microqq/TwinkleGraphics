
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
}

void FirstTriangle::UnInstall()
{
    std::cout << "Plugin UnInstall:" << _name << ".\n";
}

} // namespace TwinkleGraphics
