
#include "twView.h"

namespace TwinkleGraphics
{
View::View(Viewport &viewport)
    : _viewport(viewport)
    , _done(false)
{
}

View::~View()
{
}

void View::operator=(const Viewport &viewport)
{
    _viewport = viewport;
}

void View::Run()
{
    while(!_done)
    {
        Frame();
    }
}

void View::Frame()
{
    Advance(0.0f);
    HandleEvents();
    Render();
}

void View::Advance(float64 delta_time)
{

}

void View::Render()
{
    const Viewport &viewport = _viewport;
    glViewport(viewport.X(), viewport.Y(), viewport.Width(), viewport.Height());
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

    if(_render_routine_func != nullptr)
    {
        _render_routine_func();
    }
}

void View::HandleEvents()
{
}

} // namespace TwinkleGraphics