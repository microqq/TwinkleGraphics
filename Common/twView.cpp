
#include "twView.h"

namespace TwinkleGraphics
{
View::View()
    : _camera(nullptr)
    , _camera_control(nullptr)
    , _done(false)
{
}

View::~View()
{
    _camera_control = nullptr;
}

View& View::operator=(const Viewport &viewport)
{
    _camera->SetViewport(viewport);
    return *this;
}

View &View::operator=(const View &view)
{
    _camera->SetViewport(view._camera->GetViewport());
    _done = view._done;

    return *this;
}

void View::Run()
{
    HandleEvents();
    Advance(0.0f);
    Render();
}

void View::Advance(float64 delta_time)
{

}

void View::Render()
{
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    const Viewport &viewport = _camera->GetViewport();
    glViewport(viewport.X(), viewport.Y(), viewport.Width(), viewport.Height());
    glScissor(viewport.X(), viewport.Y(), viewport.Width(), viewport.Height());
    glEnable(GL_SCISSOR_TEST);

    glClear(viewport.clear_mask);
    if ((viewport.clear_mask & GL_COLOR_BUFFER_BIT) != 0)
    {
        const RGBA &color = viewport.clear_color;
        glClearColor(color.r, color.g, color.b, color.a);
    }
    if ((viewport.clear_mask & GL_DEPTH_BUFFER_BIT) != 0)
    {
        glClearDepth(viewport.clear_depth);
    }
    if ((viewport.clear_mask & GL_STENCIL_BUFFER_BIT) != 0)
    {
        glClearStencil(viewport.clear_stencil);
    }

    RenderImpl();

    glDisable(GL_SCISSOR_TEST);
}

void View::HandleEvents()
{
}

} // namespace TwinkleGraphics