#ifndef TW_MAINFRAME_H
#define TW_MAINFRAME_H

#include "twWidget.h"

namespace TwinkleGraphics {
class MainFrame : public Widget {
public:
  MainFrame(uint32 width, uint32 height, Widget *parent = nullptr);
  virtual ~MainFrame();
  virtual void Update(float deltaTime = 0.0f) override;

  virtual bool CheckIfClose() { return false; }
  virtual void PollInputEvents() {}

  virtual void BeginFrame() {}
  virtual void EndFrame() {}

protected:
  virtual void MouseInputCallback(int32 button, int32 action, int32 mods) {}
  virtual void CursorPosCallback(float64 xpos, float64 ypos) {}
  virtual void CursorEnterPosCallback(int32 entered) {}
  virtual void ScrollCallback(float64 dx, float64 dy) {}
  virtual void WindowSizeCallback(int32 w, int32 h) {}
  virtual void KeyInputCallBack(int32 key, int32 scannode, int32 action,
                                int32 mods) {}

protected:
};

} // namespace TwinkleGraphics

#endif
