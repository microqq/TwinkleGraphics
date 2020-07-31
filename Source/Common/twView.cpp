
#include "twView.h"

namespace TwinkleGraphics
{
View::View()
    : _camera(nullptr)
    , _cameraControl(nullptr)
    , _rect(0, 0, 1.0, 1.0)
    , _done(false)
    , _initialized(false)
{
}

View::~View()
{
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
    if(_scene != nullptr)
    {
        _scene->Update(delta_time);
    }
}

void View::Render()
{
    if(_scene != nullptr)
    {
        _scene->Render();
    }
    // [[deprecated]]
    else
    {
        const Viewport &viewport = _camera->GetViewport();
        glEnable(GL_SCISSOR_TEST);
        glScissor(viewport.X(), viewport.Y(), viewport.Width(), viewport.Height());
        glViewport(viewport.X(), viewport.Y(), viewport.Width(), viewport.Height());

        glClear(viewport.clearMask);
        if ((viewport.clearMask & GL_COLOR_BUFFER_BIT) != 0)
        {
            const RGBA &color = viewport.clearColor;
            glClearColor(color.r, color.g, color.b, color.a);
        }
        if ((viewport.clearMask & GL_DEPTH_BUFFER_BIT) != 0)
        {
            glClearDepth(viewport.clearDepth);
        }
        if ((viewport.clearMask & GL_STENCIL_BUFFER_BIT) != 0)
        {
            glClearStencil(viewport.clearStencil);
        }

        RenderImpl();

        glDisable(GL_SCISSOR_TEST);
    }
}

void View::HandleEvents()
{
}

} // namespace TwinkleGraphics