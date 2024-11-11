#include "twDockingWidget.h"
#include "twDockSpaceLayout.h"

namespace TwinkleGraphics {
DockingWidget::DockingWidget(const std::string &name, Widget *parent)
    : Widget(parent, name) {
  _pLayout = new DockSpaceLayout(this, 0.5f, 0.0f, 0.5f, 0.0f);
}

DockingWidget::~DockingWidget() {}

void DockingWidget::AttachToDockSpace(const ImGuiID &dockSpaceID) {
  _attachedDockSapceID = dockSpaceID;
}

ImGuiID DockingWidget::GetAttachedDockSpace() { return _attachedDockSapceID; }

} // namespace TwinkleGraphics
