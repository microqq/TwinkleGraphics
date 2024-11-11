#ifndef TW_EDITORAPPLICATION_H
#define TW_EDITORAPPLICATION_H

#include "twAssetsWindow.h"
#include "twDockSpaceLayout.h"
#include "twEditorMenu.h"
#include "twGLFWMainFrame.h"
#include "twGLFWViewWindow.h"
#include "twHierarchy.h"
#include "twInspector.h"

namespace TwinkleGraphics {
class __TWAPI EditorWindowLayout : public DockSpaceLayout {
public:
  explicit EditorWindowLayout(Widget *host);
  virtual ~EditorWindowLayout();

protected:
  virtual void InitSubDockSpaceNode(ImVec2 vSize) override;
  virtual void UpdateSubDockSpaceNode() override;

private:
};

class __TWAPI EditorWindow : public GLFWMainFrame {
public:
  explicit EditorWindow(uint32 width, uint32 height);
  virtual ~EditorWindow();

  virtual void Initialize() override;

protected:
  virtual void OnGuiBegin() override;
  virtual void OnGui() override;
  virtual void OnGuiEnd() override;

private:
  EditorMenu *_menu{nullptr};
  Hierarchy *_hierarchy{nullptr};
  Inspector *_inspector{nullptr};
  GLFWViewWindow *_sceneView{nullptr};
  GLFWViewWindow *_gameView{nullptr};
  AssetsWindow *_assetsBrowser{nullptr};
};

} // namespace TwinkleGraphics

#endif
