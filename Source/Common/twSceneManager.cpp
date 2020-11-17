#include "twSceneManager.h"

namespace TwinkleGraphics
{
Scene::Scene()
    : Object()
    , _maincamera(nullptr)
{}

Scene::~Scene()
{
    _cameralists.clear();
}

void Scene::Init()
{}

void Scene::Update(float32 delta_time)
{
}

void Scene::PreRender()
{
    SortCamera();
}

void Scene::Render()
{
    for(auto camera : _cameralists)
    {
    }
}

void Scene::PosRender()
{
}

void Scene::Traverse()
{

}


SceneManager::~SceneManager()
{
    _sceneLists.clear();
}

void SceneManager::RenderScene()
{
    if(_currentScene == nullptr)
    {
        return;
    }

    _currentScene->PreRender();
    _currentScene->Render();
    _currentScene->PosRender();
}

} // namespace TwinkleGraphics
