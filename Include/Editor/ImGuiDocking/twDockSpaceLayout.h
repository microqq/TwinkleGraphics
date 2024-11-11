#ifndef TW_DOCKSPACELAYOUT_H
#define TW_DOCKSPACELAYOUT_H

#include <set>
#include <string>

#include "imgui.h"
#include "twCommon.h"
#include "twAbstractLayout.h"

namespace TwinkleGraphics {
class __TWAPI DockSpaceLayout : public AbstractLayout {
public:
  explicit DockSpaceLayout(Widget *host, float leftStretch = 0.2f,
                           float rightStretch = 0.25f,
                           float downStretch = 0.362f, float upStretch = 0.2f);
  virtual ~DockSpaceLayout();

  virtual void Begin() override;
  virtual void End() override;
  virtual void AddWidget(Widget *widget,
                         Alignment alignment = ALIGN_LEFT) override;

  const ImGuiID &GetMainDockSpaceID() { return _dockSpaceID; }
  const ImGuiID &GetLeftDockSpaceID() { return _dockSpaceLeftID; }
  const ImGuiID &GetRightDockSpaceID() { return _dockSpaceRightID; }
  const ImGuiID &GetBottomDockSpaceID() { return _dockSpaceBottomID; }
  const ImGuiID &GetTopDockSpaceID() { return _dockSpaceTopID; }

  void SetImGuiViewport(ImGuiViewport *viewport) { _pImGuiViewport = viewport; }
  ImGuiViewport *GetImGuiViewport() { return _pImGuiViewport; }

protected:
  // Refer ImGuiFontStudio DockSpace
  virtual bool BeginDockSpace(ImGuiDockNodeFlags vFlags);
  virtual void EndDockSpace();
  virtual void InitSubDockSpaceNode(ImVec2 vSize);
  virtual void UpdateSubDockSpaceNode();

protected:
  std::string _dockSpaceKey;
  std::string _hostDockWindowKey;
  std::set<Widget *> _leftWidgets;
  std::set<Widget *> _rightWidgets;
  std::set<Widget *> _topWidgets;
  std::set<Widget *> _bottomWidgets;
  std::set<Widget *> _centralWidgets;
  ImGuiViewport *_pImGuiViewport{nullptr};
  float _leftStretch{0.2f};
  float _rightStretch{0.3f};
  float _downStretch{0.3f};
  float _upStretch{0.2f};
  ImGuiID _dockSpaceID;
  ImGuiID _dockSpaceLeftID;
  ImGuiID _dockSpaceRightID;
  ImGuiID _dockSpaceTopID;
  ImGuiID _dockSpaceBottomID;
  bool _initDockSpace{false};
};
} // namespace TwinkleGraphics

#endif
