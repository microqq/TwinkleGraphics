#ifndef TW_WIDGET_H
#define TW_WIDGET_H

#include <string>
#include <vector>

#include "twAbstractLayout.h"
#include "twCloseEventArgs.h"
#include "twCommon.h"
#include "twCursorEventArgs.h"
#include "twEventHandler.h"
#include "twFocusEventArgs.h"
#include "twKeyEventArgs.h"
#include "twMouseEventArgs.h"
#include "twResizeEventArgs.h"
#include "twScrollEvetnArg.h"
#include "twSizePolicy.h"

namespace TwinkleGraphics {
struct WidgetData {
  uint32 width;
  uint32 height;
  uint32 contentWidth;
  uint32 contentHeight;
  uint32 x;
  uint32 y;
};

class __TWCOMExport Widget : public Object,
                             public IUpdatable,
                             public IDestroyable,
                             public INonCopyable {
public:
  using OnGuiFunction = std::function<void()>;

  explicit Widget(Widget *parent = nullptr, const std::string &name = "");
  virtual ~Widget();
  virtual void Update(float deltaTime = 0.0f) override;
  virtual void Destroy() override;
  virtual void AttachToDockSpace(const unsigned int &dockSpaceID) {}
  virtual unsigned int GetAttachedDockSpace() { return 0; }

  void SetSizePolicy(SizePolicy sizePolicy) { _sizepolicy = sizePolicy; }
  void SetPosition(uint32 x, uint32 y) {
    _pData->x = x;
    _pData->y = y;
  }
  void SetSize(uint32 width, uint32 height) {
    _pData->width = width, _pData->height = height;
  }
  void SetWidth(uint32 width) { _pData->width = width; }
  void SetHeight(uint32 height) { _pData->height = height; }
  void SetContentWidth(uint32 contentWidth) {
    _pData->contentWidth = contentWidth;
  }
  void SetContentHeight(uint32 contentHeight) {
    _pData->contentHeight = contentHeight;
  }
  void SetEventPropagation(bool propagation) {
    _eventPropagation = propagation;
  }
  void SetLayout(AbstractLayout *layout) { _pLayout = layout; }
  void SetName(const std::string &name) { _name = name; }

  const SizePolicy &GetSizePolicy() { return _sizepolicy; }
  vec2 GetPosition() { return vec2(_pData->x, _pData->y); }
  vec2 GetSize() { return vec2(_pData->width, _pData->height); }
  uint32 GetWidth() { return _pData->width; }
  uint32 GetHeight() { return _pData->height; }
  uint32 GetContentWidth() { return _pData->contentWidth; }
  uint32 GetContentHeight() { return _pData->contentHeight; }
  bool GetEventPropagation() { return _eventPropagation; }
  AbstractLayout *GetLayout() { return _pLayout; }
  const std::string &GetName() { return _name; }

  virtual void OnGuiBegin();
  virtual void OnGui();
  virtual void OnGuiEnd();

  void PaintGui();

  void Show() { _visible = true; }
  void Hide() { _visible = false; }

  bool IsFocused() { return _focused; }
  bool IsHovered() { return _hovered; }

  void InstallEventHandler() {
    if (_eventHandlerFunc == nullptr) {
      _eventHandlerFunc = MakeClassEventHandlerFunPtr(&Widget::OnEvent, this);
      _eventHandler += _eventHandlerFunc;

      Subscribe();
    }
  }

  void UnInstallEventHandler() {
    if (_eventHandlerFunc != nullptr) {
      _eventHandler -= _eventHandlerFunc;
      UnSubscribe();
    }
  }

  void SetParent(Widget *parent = nullptr);

protected:
  virtual void OnEvent(ObjectPtr sender, BaseEventArgsPtr event);

  virtual void OnMousePressEvent(MouseEventArgs *e);
  virtual void OnMouseReleaseEvent(MouseEventArgs *e);
  virtual void OnMouseDoubleClickEvent(MouseEventArgs *e);
  virtual void OnMouseMoveEvent(MouseEventArgs *e);
  virtual void OnScrollEvent(MouseEventArgs *e);

  virtual void OnKeyPressEvent(KeyEventArgs *e);
  virtual void OnKeyReleaseEvent(KeyEventArgs *e);

  virtual void OnFocusInEvent(FocusEventArgs *e);
  virtual void OnFocusOutEvent(FocusEventArgs *e);

  virtual void OnEnterEvent(CursorEventArgs *e);
  virtual void OnLeaveEvent(CursorEventArgs *e);

  virtual void OnResizeEvent(ResizeEventArgs *e);
  virtual void OnCloseEvent(CloseEventArgs *e);

  virtual void SetFocusedInternal() {}
  virtual void SetHoveredInternal() {}

  bool HasChild(Widget *widget);
  void AddChild(Widget *widget);
  void RemoveChild(Widget *Widget);

  void SetDepth(int depth = -1) { _depth = depth; }
  int GetDepth() { return _depth; }

  void Subscribe();
  void UnSubscribe();

protected:
  std::vector<Widget *> _children;
  EventHandlerFunctionPtr _eventHandlerFunc{nullptr};
  WidgetData *_pData{nullptr};
  Widget *_parent{nullptr};
  AbstractLayout *_pLayout{nullptr};
  EventHandler _eventHandler;
  SizePolicy _sizepolicy;
  std::string _name;
  int _depth{-1};
  bool _visible{true};
  bool _focused{false};
  bool _hovered{false};
  bool _needResize{false};
  bool _eventPropagation{true};
};
} // namespace TwinkleGraphics

#endif
