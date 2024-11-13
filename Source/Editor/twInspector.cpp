#include "imgui.h"

#include "twInspector.h"


namespace TwinkleGraphics {
Inspector::Inspector(const std::string &name, Widget *parent)
    : Widget(parent, name) {}

Inspector::~Inspector() {}

void Inspector::OnGuiBegin() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::Begin(_name.c_str(), NULL,
               ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoBringToFrontOnFocus |
                   ImGuiWindowFlags_NoNavFocus);
}

void Inspector::OnGui() {}

void Inspector::OnGuiEnd() {
  ImGui::End();
  ImGui::PopStyleVar();
}

} // namespace TwinkleGraphics
