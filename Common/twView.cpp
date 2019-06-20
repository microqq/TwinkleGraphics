
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

View& View::operator=(const Viewport &viewport)
{
    _viewport = viewport;
    return *this;
}

View &View::operator=(const View &view)
{
    _viewport = view._viewport;
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
    glViewport(_viewport.X(), _viewport.Y(), _viewport.Width(), _viewport.Height());
    glScissor(_viewport.X(), _viewport.Y(), _viewport.Width(), _viewport.Height());
    glEnable(GL_SCISSOR_TEST);

    glClear(_viewport.clear_mask);
    if ((_viewport.clear_mask & GL_COLOR_BUFFER_BIT) != 0)
    {
        const RGBA &color = _viewport.clear_color;
        glClearColor(color.r, color.g, color.b, color.a);
    }
    if ((_viewport.clear_mask & GL_DEPTH_BUFFER_BIT) != 0)
    {
        glClearDepth(_viewport.clear_depth);
    }
    if ((_viewport.clear_mask & GL_STENCIL_BUFFER_BIT) != 0)
    {
        glClearStencil(_viewport.clear_stencil);
    }

    RenderImpl();

    glDisable(GL_SCISSOR_TEST);
}

void View::HandleEvents()
{
}

} // namespace TwinkleGraphics