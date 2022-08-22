#include "twAssetsWindow.h"
#include "imgui.h"

namespace TwinkleGraphics {
bool showDemoWindow = true;

AssetsWindow::AssetsWindow(const std::string &name, uint32 width, uint32 height,
                           Widget *parent)
    : Window(name, width, height, parent) {}

AssetsWindow::~AssetsWindow() {}

void AssetsWindow::Update(float deltaTime) {}

void AssetsWindow::OnGuiBegin() {
  // ImGui::SetNextWindowSize(ImVec2(_pData->width, _pData->height));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::Begin(_name.c_str(), NULL,
               ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
}

void AssetsWindow::OnGui() {
  ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
  ImVec2 contentMax = ImGui::GetWindowContentRegionMax();
  ImVec2 contentSize(contentMax.x - contentMin.x, contentMax.y - contentMin.y);

  // subwindow Project Tree
  ImGui::Columns(2);
  // float columnWidth = ImGui::GetColumnWidth();
  static bool setColumnWidth = false;
  if (!setColumnWidth) {
    ImGui::SetColumnWidth(0, contentSize.x * 0.3f);
    ImGui::SetColumnWidth(1, contentSize.x * 0.7f);
    setColumnWidth = true;
  }
  ImGui::BeginChild(u8"Project Folder",
                    ImVec2(contentSize.x * 0.3f, contentSize.y), false,
                    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
  ImGui::EndChild();

  // subwindow Assets Preview
  ImGui::NextColumn();
  // columnWidth = ImGui::GetColumnWidth();
  ImGui::BeginChild(u8"Assets Preview",
                    ImVec2(contentSize.x * 0.7f, contentSize.y), false,
                    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
  ImGui::EndChild();
}

void AssetsWindow::OnGuiEnd() {
  SetFocusedInternal();
  SetHoveredInternal();

  ImGui::End();
  ImGui::PopStyleVar();

  // ImGui::ShowDemoWindow(&showDemoWindow);
}

void AssetsWindow::SetFocusedInternal() {
  bool focused = ImGui::IsWindowFocused(ImGuiHoveredFlags_RootAndChildWindows);
  if (_focused != focused) {
    _focused = focused;
    if (_focused) {
      // focusedin
    } else {
      // focusedout
    }
    // Console::LogInfo("Assets window focused:", _focused, "\n");
  }
}

void AssetsWindow::SetHoveredInternal() {
  bool hovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
  if (_hovered != hovered) {
    _hovered = hovered;
    if (_hovered) {

    } else {
      /* code */
    }
    // Console::LogInfo("Assets window hovered:", _hovered, "\n");
  }
}
} // namespace TwinkleGraphics
