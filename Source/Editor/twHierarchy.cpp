#include "imgui.h"

#include "twHierarchy.h"

namespace TwinkleGraphics {
Hierarchy::Hierarchy(const std::string &name, Widget *parent)
    : Widget(parent, name) {}

Hierarchy::~Hierarchy() {}

void Hierarchy::OnGuiBegin() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::Begin(_name.c_str(), NULL,
               ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoBringToFrontOnFocus |
                   ImGuiWindowFlags_NoNavFocus);
}

void Hierarchy::OnGui() {
  if (ImGui::Button("Click Me")) {
    Console::LogInfo("Hierarchy button clicked!\n");
  }
}

void Hierarchy::OnGuiEnd() {
  ImGui::End();
  ImGui::PopStyleVar();
}

} // namespace TwinkleGraphics
