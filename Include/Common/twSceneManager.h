

#ifndef TW_SCENEMANAGEMENT_H
#define TW_SCENEMANAGEMENT_H

#include "twCamera.h"
#include "twCommon.h"

#define MAX_SCENE_CAMERA_COUNT 128

namespace TwinkleGraphics {

class __TWAPI Scene : public Object {
public:
  using Ptr = std::shared_ptr<Scene>;

  Scene();
  virtual ~Scene();

  virtual void Update(float32 deltaTime = 0.0f);
  virtual void Init();
  virtual void Render() {}

  void SetMainCamera(CameraPtr cam) {
    _maincamera = cam;
    AddCamera(cam);
  }

  void AddCamera(CameraPtr cam) {
    if (cam == nullptr)
      return;

    if (_cameralists.size() == MAX_SCENE_CAMERA_COUNT) {
      return;
    }

    std::vector<CameraPtr>::iterator findIter =
        std::find_if(_cameralists.begin(), _cameralists.end(),
                     [cam](CameraPtr ele) { return cam == ele; });
    if (findIter != _cameralists.end()) {
      _cameralists.emplace_back(cam);
      _cameraSorted = false;
    }
  }

  void RemoveCamera(int32 index) {
    if (index < 0 || index >= MAX_SCENE_CAMERA_COUNT) {
      return;
    }

    _cameralists.erase(_cameralists.begin() + index);
  }

  void RemoveCamera(CameraPtr cam) {
    if (cam == nullptr) {
      return;
    }

    std::vector<CameraPtr>::iterator findIter =
        std::find_if(_cameralists.begin(), _cameralists.end(),
                     [cam](CameraPtr ele) { return cam == ele; });
    if (findIter != _cameralists.end()) {
      _cameralists.erase(findIter);
    }
  }

  CameraPtr GetMainCamera() { return _maincamera; }
  CameraPtr GetCamera(int32 index) {
    if (index < 0 || index >= MAX_SCENE_CAMERA_COUNT) {
      return nullptr;
    }
    return _cameralists[index];
  }

  int32 GetCameraCount() { return _cameralists.size(); }

  SceneNodePtr GetRootNode() { return _rootNode; }

private:
  void SortCamera() {
    if (!_cameraSorted) {
      std::sort(_cameralists.begin(), _cameralists.end(),
                [](CameraPtr a, CameraPtr b) {
                  return a->GetDepth() < b->GetDepth();
                });

      _cameraSorted = true;
    }
  }

protected:
  std::vector<CameraPtr> _cameralists;
  CameraPtr _maincamera{nullptr};
  SceneNodePtr _rootNode{nullptr};

  bool _cameraSorted = false;
};

class SceneManager : public IUpdatable,
                     public INonCopyable,
                     public IDestroyable {
public:
  virtual ~SceneManager();

  /**
   * @brief
   * Update() must execute in main thread
   */
  virtual void Update(float deltaTime = 0.0f) override;
  virtual void Destroy() override {}

private:
  explicit SceneManager() : IUpdatable(), INonCopyable(), IDestroyable() {}

private:
  std::vector<Scene::Ptr> _sceneLists;
  Scene::Ptr _currentScene{nullptr};

  friend class Singleton<SceneManager>;
};

} // namespace TwinkleGraphics

#endif