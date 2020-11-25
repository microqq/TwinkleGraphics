#include "twSceneManager.h"

namespace TwinkleGraphics
{
Scene::Scene()
    : Object()
    , _cameralists()
    , _maincamera(nullptr)
    , _rootNode(nullptr)
{
    _rootNode = std::make_shared<SceneNode>();
}

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
    _rootNode->Traverse();
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
