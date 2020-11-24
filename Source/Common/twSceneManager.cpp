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
    TraverseScene();
}

void Scene::TraverseScene()
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
