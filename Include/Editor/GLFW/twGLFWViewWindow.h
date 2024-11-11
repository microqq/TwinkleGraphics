#ifndef TW_VIEWWINDOW_H
#define TW_VIEWWINDOW_H

#include "imgui.h"
#include "twRenderTexture.h"
#include "twWindow.h"

#include "twSceneManager.h"

namespace TwinkleGraphics {
class __TWAPI GLFWViewWindow : public Window {
public:
  explicit GLFWViewWindow(const std::string &name, uint32 width, uint32 height,
                          Widget *parent = nullptr);
  virtual ~GLFWViewWindow();
  virtual void Update(float deltaTime) override;

  RenderTexturePtr GetViewRT() { return _viewRT; }

  void LoadScene(Scene::Ptr scene) {
    if (_currentScene == scene) {
      return;
    }

    if (_currentScene != nullptr) {
      UnloadScene();
    }

    // TODO: load scene
    _currentScene = scene;
  }

  // TODO: unload current scene
  void UnloadScene() {
    if (_currentScene != nullptr) {
    }
  }

protected:
  virtual void OnGuiBegin() override;
  virtual void OnGui() override;
  virtual void OnGuiEnd() override;

private:
  virtual void SetFocusedInternal() override;
  virtual void SetHoveredInternal() override;
  void CreateViewRT();
  void PaintViewGui(ImVec2 viewSize);

private:
  RenderTexturePtr _viewRT{nullptr};
  Scene::Ptr _currentScene{nullptr};
};
} // namespace TwinkleGraphics

#endif