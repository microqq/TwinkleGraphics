#include "twSceneManagement.h"

namespace TwinkleGraphics
{
Scene::Scene()
    : Object()
    , _maincamera(nullptr)
    , _valid_camera_count(-1)
{}

Scene::~Scene()
{

}

void Scene::Init()
{}

void Scene::Update(float32 delta_time)
{}

void Scene::Render()
{
    if(_maincamera != nullptr)
    {
        const Viewport &viewport = _maincamera->GetViewport();
        glEnable(GL_SCISSOR_TEST);
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
    }        
}

} // namespace TwinkleGraphics
