
#include "twDockSpaceLayout.h"
#include "imgui.h"

#include "imgui_internal.h"

#include "twComUtil.h"
#include "twWidget.h"

namespace TwinkleGraphics {
DockSpaceLayout::DockSpaceLayout(Widget *host, float leftStretch,
                                 float rightStretch, float downStretch,
                                 float upStretch)
    : AbstractLayout(host), _leftStretch(leftStretch),
      _rightStretch(rightStretch), _downStretch(downStretch),
      _upStretch(upStretch) {
  _dockSpaceKey = RandomString();
  _hostDockWindowKey = RandomString();
}

DockSpaceLayout::~DockSpaceLayout() {
  _leftWidgets.clear();
  _rightWidgets.clear();
  _topWidgets.clear();
  _bottomWidgets.clear();
  _centralWidgets.clear();
}

void DockSpaceLayout::Begin() {
  BeginDockSpace(ImGuiDockNodeFlags_PassthruCentralNode);
  EndDockSpace();

  if (!_initDockSpace) {
    ImVec2 vSize = ImVec2(_hostWidget->GetWidth(), _hostWidget->GetHeight());
    InitSubDockSpaceNode(vSize);
    _initDockSpace = true;
  }

  UpdateSubDockSpaceNode();
}

void DockSpaceLayout::End() {}

void DockSpaceLayout::AddWidget(Widget *widget, Alignment alignment) {
  switch (alignment) {
  case ALIGN_LEFT:
    _leftWidgets.emplace(widget);
    break;
  case ALIGN_RIGHT:
    _rightWidgets.emplace(widget);
    break;
  case ALIGN_TOP:
    _topWidgets.emplace(widget);
    break;
  case ALIGN_BOTTOM:
    _bottomWidgets.emplace(widget);
    break;
  case ALIGN_CENTER:
    _centralWidgets.emplace(widget);
    break;
  default:
    break;
  }
}

bool DockSpaceLayout::BeginDockSpace(ImGuiDockNodeFlags vFlags) {
  ImVec2 vPos, vSize;
  ImGuiID attachedDockSpaceID = _hostWidget->GetAttachedDockSpace();
  if (_pImGuiViewport != nullptr && attachedDockSpaceID == 0) {
    vPos = _pImGuiViewport->WorkPos;
    vSize = _pImGuiViewport->WorkSize;

  } else {
    if (attachedDockSpaceID != 0) {
      auto dockNode = ImGui::DockContextFindNodeByID(ImGui::GetCurrentContext(),
                                                     attachedDockSpaceID);
      vPos = dockNode->Pos;
      vSize = dockNode->Size;
    }
  }

  auto context = ImGui::GetCurrentContext();
  IM_ASSERT(context != nullptr);
  ImGui::SetNextWindowPos(vPos);
  ImGui::SetNextWindowSize(vSize);
  if (_pImGuiViewport == nullptr) {
    _pImGuiViewport = ImGui::GetMainViewport();
  }
  ImGui::SetNextWindowViewport(_pImGuiViewport->ID);

  auto hostWindowFlags = 0;
  hostWindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoDocking;
  hostWindowFlags |=
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  if (vFlags & ImGuiDockNodeFlags_PassthruCentralNode)
    hostWindowFlags |= ImGuiWindowFlags_NoBackground;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  // char label[100 + 1];
  // ImFormatString(label, 100, "DockSpaceViewport_%08X", _pImGuiViewport->ID);
  // const auto res = ImGui::Begin(label, nullptr, hostWindowFlags);
  const auto res =
      ImGui::Begin(_hostDockWindowKey.c_str(), nullptr, hostWindowFlags);
  ImVec2 hostSize = ImGui::GetWindowSize();
  ImVec2 hostPos = ImGui::GetWindowPos();
  _hostWidget->SetSize(hostSize.x, hostSize.y);
  _hostWidget->SetPosition(hostPos.x, hostPos.y);

  _dockSpaceID = ImGui::GetID(_dockSpaceKey.c_str());
  ImGui::DockSpace(_dockSpaceID, ImVec2(0.0f, 0.0f), vFlags);

  return res;
}

void DockSpaceLayout::EndDockSpace() 
{ 
  ImGui::End();
  ImGui::PopStyleVar(3);
}

void DockSpaceLayout::InitSubDockSpaceNode(ImVec2 vSize) {
  auto size = ImVec2((float)vSize.x, (float)vSize.y);
#define IS_FLOAT_EQUAL(a, b) (fabs((a) - (b)) < FLT_EPSILON)

  if (IS_FLOAT_EQUAL(size.x, 0.0) || IS_FLOAT_EQUAL(size.y, 0.0)) {
    return;
  }
#undef IS_FLOAT_EQUAL

  ImGui::DockBuilderRemoveNode(_dockSpaceID); // Clear out existing layout
  ImGui::DockBuilderAddNode(_dockSpaceID,
                            ImGuiDockNodeFlags_DockSpace); // Add empty node
  ImGui::DockBuilderSetNodeSize(_dockSpaceID, vSize);

  auto dockMainID = _dockSpaceID; // This variable will track the document
                                  // node, however we are not using it here as
                                  // we aren't docking anything into it.

  ImGuiID tempDockSpaceID1{0}, tempDockSpaceID2{0};
  _dockSpaceLeftID = ImGui::DockBuilderSplitNode(
      dockMainID, ImGuiDir_Left, _leftStretch, nullptr, &tempDockSpaceID1);
  _dockSpaceBottomID =
      ImGui::DockBuilderSplitNode(tempDockSpaceID1, ImGuiDir_Down, _downStretch,
                                  nullptr, &tempDockSpaceID2);
  _dockSpaceRightID =
      ImGui::DockBuilderSplitNode(tempDockSpaceID2, ImGuiDir_Right,
                                  _rightStretch, nullptr, &tempDockSpaceID1);
  _dockSpaceTopID = ImGui::DockBuilderSplitNode(
      tempDockSpaceID1, ImGuiDir_Up, _upStretch, nullptr, &tempDockSpaceID2);

  _dockSpaceID = tempDockSpaceID2;
}

void DockSpaceLayout::UpdateSubDockSpaceNode() {
  bool empty = _leftWidgets.empty() && _rightWidgets.empty() &&
               _topWidgets.empty() && _bottomWidgets.empty() &&
               _centralWidgets.empty();

  if (empty) {
    return;
  }

  // update center dockspace node
  for (const auto &widget : _centralWidgets) {
    widget->AttachToDockSpace(_dockSpaceID);
    ImGui::DockBuilderDockWindow(widget->GetName().c_str(), _dockSpaceID);
  }
  _centralWidgets.clear();

  // update left dockspace node
  for (const auto &widget : _leftWidgets) {
    widget->AttachToDockSpace(_dockSpaceLeftID);
    ImGui::DockBuilderDockWindow(widget->GetName().c_str(), _dockSpaceLeftID);
  }
  _leftWidgets.clear();

  // update right dockspace node
  for (const auto &widget : _rightWidgets) {
    widget->AttachToDockSpace(_dockSpaceRightID);
    ImGui::DockBuilderDockWindow(widget->GetName().c_str(), _dockSpaceRightID);
  }
  _rightWidgets.clear();

  // update top dockspace node
  for (const auto &widget : _topWidgets) {
    widget->AttachToDockSpace(_dockSpaceTopID);
    ImGui::DockBuilderDockWindow(widget->GetName().c_str(), _dockSpaceTopID);
  }
  _topWidgets.clear();

  // update bottom dockspace node
  for (const auto &widget : _bottomWidgets) {
    widget->AttachToDockSpace(_dockSpaceBottomID);
    ImGui::DockBuilderDockWindow(widget->GetName().c_str(), _dockSpaceBottomID);
  }
  _bottomWidgets.clear();

  ImGui::DockBuilderFinish(_dockSpaceID);
}

} // namespace TwinkleGraphics
