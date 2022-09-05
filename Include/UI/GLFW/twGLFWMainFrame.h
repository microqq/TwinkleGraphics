#ifndef TW_GLFWMAINFRAME_H
#define TW_GLFWMAINFRAME_H

#include "imgui.h"
#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "twMainFrame.h"

namespace TwinkleGraphics {
class __TWCOMExport GLFWMainFrame : public MainFrame {
public:
  explicit GLFWMainFrame(uint32 width, uint32 height, Widget *parent = nullptr);
  virtual ~GLFWMainFrame();

  virtual void Update(float deltaTime = 0.0f) override;
  virtual void Destroy() override;

  virtual bool CheckIfClose() override;
  virtual void PollInputEvents() override;

  virtual void BeginFrame() override;
  virtual void EndFrame() override;

  virtual void OnGuiBegin() override;
  virtual void OnGuiEnd() override;

protected:
  virtual void MouseInputCallback(int32 button, int32 action,
                                  int32 mods) override;
  virtual void CursorPosCallback(float64 xpos, float64 ypos) override;
  virtual void CursorEnterPosCallback(int32 entered) override;
  virtual void ScrollCallback(float64 dx, float64 dy) override;
  virtual void WindowSizeCallback(int32 w, int32 h) override;
  virtual void KeyInputCallBack(int32 key, int32 scannode, int32 action,
                                int32 mods) override;
  virtual void SetInputEventCallbacks();

protected:
  GLFWwindow *_window{nullptr};
};
} // namespace TwinkleGraphics

#endif
