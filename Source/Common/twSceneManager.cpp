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
    SortCamera();
    Traverse();
}

void Scene::Render()
{
    for(auto camera : _cameralists)
    {
    }
}

void Scene::Traverse()
{

}


SceneManager::~SceneManager()
{
    _sceneLists.clear();
}


void SceneManager::Update(float deltaTime)
{
    if(_currentScene == nullptr)
    {
        return;
    }

    _currentScene->Update(deltaTime);
}

} // namespace TwinkleGraphics
