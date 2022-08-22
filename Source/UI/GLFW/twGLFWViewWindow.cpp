#include "twGLFWViewWindow.h"

namespace TwinkleGraphics {
GLFWViewWindow::GLFWViewWindow(const std::string &name, uint32 width,
                               uint32 height, Widget *parent)
    : Window(name, width, height, parent) {}

GLFWViewWindow::~GLFWViewWindow() {}

void GLFWViewWindow::Update(float deltaTime) {}

void GLFWViewWindow::OnGuiBegin() {
  // https://github.com/ocornut/imgui/issues/1490
  // specify content window size with PushStyleVar --
  // ImGuiStyleVar_WindowPadding
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  // ImGui::SetNextWindowSize(ImVec2(_pData->width, _pData->height));
  ImGui::Begin(_name.c_str(), NULL,
               ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoBringToFrontOnFocus |
                   ImGuiWindowFlags_NoNavFocus);
  // ImGui::Text(_name.c_str());
  // ImGui::Separator();
}

void GLFWViewWindow::OnGuiEnd() {
  SetFocusedInternal();
  SetHoveredInternal();

  ImGui::End();
  ImGui::PopStyleVar();
}

void GLFWViewWindow::OnGui() {
  // ImVec2 size = ImGui::GetWindowSize();

  if (!ImGui::IsWindowCollapsed()) {
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 viewSize = ImVec2(0.0f, 0.0f);

    // TODO: use Resize Event
    ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
    ImVec2 contentMax = ImGui::GetWindowContentRegionMax();
    ImVec2 contentSize(contentMax.x - contentMin.x,
                       contentMax.y - contentMin.y);
    ImVec2 titleBarSize(windowSize.x - contentSize.x,
                        windowSize.y - contentSize.y);

    // set widget width/height
    SetContentWidth(contentSize.x);
    SetContentHeight(contentSize.y);

    if (viewSize.x != contentSize.x || viewSize.y != contentSize.y) {
      viewSize = contentSize;
      viewSize.x -= 2.f;
      viewSize.y -= 2.f;
    }

    // when imgui view window created, create view render texture.
    CreateViewRT();

    _pData->width = windowSize.x;
    _pData->height = windowSize.y;

    // render scene
    if (_currentScene != nullptr) {
      _currentScene->Update(.0f);
      _currentScene->Render();
    }

    // paint view rt
    PaintViewGui(viewSize);
  }
}

void GLFWViewWindow::SetFocusedInternal() {
  bool focused = ImGui::IsWindowFocused(ImGuiHoveredFlags_RootAndChildWindows);
  if (_focused != focused) {
    _focused = focused;
    if (_focused) {
      // focusedin
    } else {
      // focusedout
    }
    // Console::LogInfo("View window focused:", _focused, "\n");
  }
}

void GLFWViewWindow::SetHoveredInternal() {
  bool hovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
  if (_hovered != hovered) {
    _hovered = hovered;
    if (_hovered) {
    } else {
      /* code */
    }
    // Console::LogInfo("View window hovered:", _hovered, "\n");
  }
}

void GLFWViewWindow::CreateViewRT() {
  if (_viewRT != nullptr)
    return;

  _viewRT = std::make_shared<RenderTexture>(1024.0f, 1024.0f, GL_RGBA8, GL_RGBA,
                                            false, false, false, 1, false);
  assert(_viewRT != nullptr);

  _viewRT->Create(nullptr);
  _viewRT->GetTexture()->SetFilter<FilterParam::MIN_FILTER>(FilterMode::LINEAR);
  _viewRT->GetTexture()->SetFilter<FilterParam::MAG_FILTER>(FilterMode::LINEAR);
}

void GLFWViewWindow::PaintViewGui(ImVec2 viewSize) {
  ImGui::BeginChild(u8"SubWindow", viewSize, false,
                    ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar
                    // | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove
  );

  {
    TexturePtr tex = _viewRT->GetTexture();
    uint id = tex->GetRenderRes().id;
    ImGui::Image((ImTextureID)id, viewSize, ImVec2(0.0f, 1.0f),
                 ImVec2(1.0f, 0.0f));
  }
  ImGui::EndChild();
}

} // namespace TwinkleGraphics
