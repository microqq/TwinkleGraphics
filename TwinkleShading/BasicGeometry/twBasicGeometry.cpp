
#include <iostream>

#include "imgui.h"
#include "twMesh.h"

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
}

void BasicGeometry::Install()
{
    Plugin::Install();

    // Initilize view
    Viewport viewport(Rect(0, 0, 1024, 768), 17664U, RGBA(0.0f, 0.f, 0.f, 1.f));
    _view = new BasicGeometryView(viewport);
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
    Mesh::Ptr mesh = Mesh::CreateSphereMeshStandard(1.0f, 20, 10);
}

void BasicGeometryView::RenderImpl()
{
}

void BasicGeometryView::OnGUI()
{
}

void BasicGeometryView::Destroy()
{
}

} // namespace TwinkleGraphics
