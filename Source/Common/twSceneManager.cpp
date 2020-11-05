#include "twSceneManager.h"

namespace TwinkleGraphics
{
Scene::Scene()
    : Object()
    , _maincamera(nullptr)
    , _validCameraCount(-1)
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
    // [[deprecated]]
    // if(_maincamera != nullptr)
    // {
    //     _maincamera->ClearRenderContext();
    // }        
}




SceneManager::~SceneManager()
{}

} // namespace TwinkleGraphics
