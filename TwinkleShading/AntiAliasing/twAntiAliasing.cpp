

#include "imgui.h"

#include "twAntiAliasing.h"

namespace TwinkleGraphics
{
AntiAliasing::AntiAliasing(std::string& name)
    : GLPlugin(name)
    , _view(nullptr)
    , _camera_control(nullptr)
{}

AntiAliasing::~AntiAliasing()
{
    SAFE_DEL(_view);
    SAFE_DEL(_camera_control);
}

void AntiAliasing::Install()
{
    Plugin::Install();

    // Initilize view
    Viewport viewport(Rect(0, 0, 800, 640), 17664U, RGBA(0.0f, 0.f, 0.f, 1.f));
    Camera::Ptr camera = std::make_shared<Camera>(viewport, 45.0f, 0.1f, 1000.0f);
    _view = new AntiAliasingView();
    _view->SetViewCamera(camera);
    _camera_control = new OrbitControl(camera);
    (dynamic_cast<OrbitControl*>(_camera_control))->SetMinDistance(1.0f);
    _view->SetCameraControl(_camera_control);
    _view->Initialize();

    _views[_views_count++] = _view;
}

void AntiAliasing::UnInstall()
{
    _view->Destroy();
    SAFE_DEL(_view);

    Plugin::UnInstall();
}

void AntiAliasingView::Initialize()
{
}
void AntiAliasingView::Destroy()
{
}
void AntiAliasingView::Advance(float64 delta_time)
{
}
void AntiAliasingView::RenderImpl()
{
}
void AntiAliasingView::OnGUI()
{
    ImGui::Begin(u8"反走样");
    {
        
    }
    ImGui::End();
}

} // namespace TwinkleGraphics
