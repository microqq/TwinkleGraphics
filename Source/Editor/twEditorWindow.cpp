#include <assert.h>

#include "twEditorWindow.h"
#include "twDockSpaceLayout.h"
#include "twDockingWidget.h"
#include "twImGuiContextManager.h"

namespace TwinkleGraphics {
////////////////////////////EditorWindowLayout///////////////////////////////
EditorWindowLayout::EditorWindowLayout(Widget *host) : DockSpaceLayout(host) {}

EditorWindowLayout::~EditorWindowLayout() {}

void EditorWindowLayout::InitSubDockSpaceNode(ImVec2 vSize) {
  DockSpaceLayout::InitSubDockSpaceNode(vSize);
}

void EditorWindowLayout::UpdateSubDockSpaceNode() {
  DockSpaceLayout::UpdateSubDockSpaceNode();
}

////////////////////////////EditorWindow///////////////////////////////
EditorWindow::EditorWindow(uint32 width, uint32 height)
    : GLFWMainFrame(width, height) {
  this->SetName("twinkle editor");
}

void EditorWindow::Initialize() {
  GLFWMainFrame::Initialize();

  const std::string &assetsDir =
      std::filesystem::current_path().string() + "/Assets";
  _assetsBrowser = new AssetsWindow("Assets Browser", _pData->width * 0.5f,
                                    _pData->height * 0.25f, assetsDir, this);
  _hierarchy = new Hierarchy("Hierarchy", this);
  _inspector = new Inspector("Inspector", this);
  _menu = new EditorMenu(this);

  const auto &mainframeLayout = dynamic_cast<DockSpaceLayout *>(_pLayout);
  mainframeLayout->AddWidget(_hierarchy, ALIGN_LEFT);
  mainframeLayout->AddWidget(_assetsBrowser, ALIGN_BOTTOM);
  mainframeLayout->AddWidget(_inspector, ALIGN_RIGHT);

  const auto centralWidget = new DockingWidget("central docking widget", this);
  mainframeLayout->AddWidget(centralWidget, ALIGN_CENTER);

  _sceneView = new GLFWViewWindow("Scene View", _pData->width * 0.25f,
                                  _pData->height * 0.25f, centralWidget);
  _gameView = new GLFWViewWindow("Game View", _pData->width * 0.25f,
                                 _pData->height * 0.25f, centralWidget);
  const auto centralWidgetLayout =
      dynamic_cast<DockSpaceLayout *>(centralWidget->GetLayout());
  centralWidgetLayout->AddWidget(_sceneView, ALIGN_LEFT);
  centralWidgetLayout->AddWidget(_gameView, ALIGN_CENTER);  
}

void EditorWindow::OnGuiBegin() {
  //
  GLFWMainFrame::OnGuiBegin();
}

void EditorWindow::OnGui() {
  //
  GLFWMainFrame::OnGui();
}

void EditorWindow::OnGuiEnd() {
  //
  GLFWMainFrame::OnGuiEnd();
}

EditorWindow::~EditorWindow() {
  _sceneView = nullptr;
  _gameView = nullptr;
  _assetsBrowser = nullptr;

  _menu = nullptr;
  _hierarchy = nullptr;
  _inspector = nullptr;
}

} // namespace TwinkleGraphics
