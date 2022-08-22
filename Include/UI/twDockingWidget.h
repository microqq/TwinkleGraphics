#ifndef TW_DOCKING_WIDGET_H
#define TW_DOCKING_WIDGET_H

#include "imgui.h"
#include "twWidget.h"

namespace TwinkleGraphics {
class DockingWidget : public Widget {
public:
  explicit DockingWidget(const std::string &name, Widget *parent);
  virtual ~DockingWidget();

  virtual void AttachToDockSpace(const ImGuiID &dockSpaceID) override;
  virtual ImGuiID GetAttachedDockSpace() override;

private:
  ImGuiID _attachedDockSapceID{0};
  bool _initHostDockWindow{false};
};

} // namespace TwinkleGraphics
#endif