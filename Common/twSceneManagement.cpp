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

void Scene::Update()
{}

void Scene::Render()
{}

} // namespace TwinkleGraphics
