

#ifndef TW_MainWINDOW_H
#define TW_MainWINDOW_H

#include <iostream>
#include <vector>

#include "imgui.h"
#include "twView.h"
#include <glfw/glfw3.h>

namespace TwinkleGraphics {

class __TWCOMExport MainWindow {
public:
  MainWindow(int32 width = 800, int32 height = 640);
  virtual ~MainWindow();

  void AddViews(View **views, int num);
  void AddView(View *view);
  void RemoveViews(View **view, int num);
  void RemoveView(View *view);

  virtual void Reset() = 0;
  virtual void Run() = 0;

  virtual void MouseButtonInput(int32 button, int32 action, int32 mods) {}
  virtual void CursorPosCallback(float64 xpos, float64 ypos) {}
  virtual void CursorEnterPosCallback(int32 entered) {}
  virtual void ScrollCallback(float64 dx, float64 dy) {}
  virtual void WindowSizeCallback(int32 w, int32 h) {
    if (_width != w || _height == h) {
      _windowResize = true;
      _width = w;
      _height = h;
    }
  }
  virtual void KeyCallBack(int32 key, int32 scannode, int32 action,
                           int32 mods) {}

protected:
  virtual void Initialise() = 0;
  virtual void Terminate() = 0;
  virtual void HandleEvents() = 0;

  virtual void MouseLeftButtonClick(glm::dvec2 pos) = 0;
  virtual void MouseLeftButtonRelease(glm::dvec2 pos) = 0;
  virtual void MouseRightButtonClick(glm::dvec2 pos) = 0;
  virtual void MouseRightButtonRelease(glm::dvec2 pos) = 0;
  virtual void MouseMiddleButtonClick(glm::dvec2 pos) = 0;
  virtual void MouseMiddleButtonRelease(glm::dvec2 pos) = 0;
  virtual void MouseMove(glm::dvec2 pos) = 0;
  virtual void MouseLeftButtonDragMove(glm::dvec2 pos) = 0;
  virtual void MouseRightButtonDragMove(glm::dvec2 pos) = 0;
  virtual void MouseMiddleButtonDragMove(glm::dvec2 pos) = 0;
  virtual void MouseScroll(glm::dvec2 scroll) = 0;
  virtual void MouseDoubleClick() = 0;
  virtual void KeyPress(int32 key, int32 scannode, int32 action,
                        int32 mods) = 0;
  virtual void KeyRelease(int32 key, int32 scannode, int32 action,
                          int32 mods) = 0;

protected:
  View *_views[MAX_VIEWPORT_COUNT];

  glm::dvec2 _cursorEnterPos;
  glm::dvec2 _cursorPos;

  int32 _width;
  int32 _height;
  int8 _viewCount;

  bool _leftButtonDown;
  bool _rightButtonDown;
  bool _middleButtonDown;
  bool _leftButtonDragmove;
  bool _rightButtonDragmove;
  bool _middleButtonDragmove;
  bool _mouseMoving;
  bool _cursorEnterd;
  bool _windowResize;
};

class __TWCOMExport GLFWMainWindow : public MainWindow {
public:
  GLFWMainWindow(int32 width = 800, int32 height = 640);
  virtual ~GLFWMainWindow();

  void AddGUIFunc(IMGUI_FUNC func);
  void SetMouseButtonInputCallback(GLFWmousebuttonfun func) {
    glfwSetMouseButtonCallback(_window, func);
  }
  void SetCursorPosCallback(GLFWcursorposfun func) {
    glfwSetCursorPosCallback(_window, func);
  }
  void SetCursorPosEnterCallback(GLFWcursorenterfun func) {
    glfwSetCursorEnterCallback(_window, func);
  }
  void SetScrollCallback(GLFWscrollfun func) {
    glfwSetScrollCallback(_window, func);
  }
  void SetWindowSizeCallback(GLFWwindowsizefun func) {
    glfwSetWindowSizeCallback(_window, func);
  }
  void SetKeyCallback(GLFWkeyfun func) { glfwSetKeyCallback(_window, func); }

  virtual void Reset() override;
  virtual void Run() override;

  virtual void MouseButtonInput(int32 button, int32 action,
                                int32 mods) override {
    glfwGetCursorPos(_window, &(_cursorPos.x), &(_cursorPos.y));

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
      if (action == GLFW_PRESS) {
        MouseLeftButtonClick(_cursorPos);
      } else if (action == GLFW_RELEASE) {
        MouseLeftButtonRelease(_cursorPos);
      }
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
      if (action == GLFW_PRESS) {
        MouseRightButtonClick(_cursorPos);
      } else if (action == GLFW_RELEASE) {
        MouseRightButtonRelease(_cursorPos);
      }
    } else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
      if (action == GLFW_PRESS) {
        MouseMiddleButtonClick(_cursorPos);
      } else if (action == GLFW_RELEASE) {
        MouseMiddleButtonRelease(_cursorPos);
      }
    }
  }
  virtual void CursorPosCallback(float64 xpos, float64 ypos) override {
    if (_leftButtonDown) {
      MouseLeftButtonDragMove(glm::dvec2(xpos, ypos));
    } else if (_rightButtonDown) {
      MouseRightButtonDragMove(glm::dvec2(xpos, ypos));
    } else if (_middleButtonDown) {
      _middleButtonDragmove = true;
    }

    glfwGetCursorPos(_window, &(_cursorPos.x), &(_cursorPos.y));
  }

  virtual void CursorEnterPosCallback(int32 entered) override {
    if (entered) {
      _cursorEnterd = true;
      glfwGetCursorPos(_window, &(_cursorEnterPos.x), &(_cursorEnterPos.y));
    } else {
      _cursorEnterd = false;
    }
  }

  virtual void ScrollCallback(float64 dx, float64 dy) override {
    MouseScroll(glm::dvec2(dx, dy));
    return;
  }

  virtual void WindowSizeCallback(int32 w, int32 h) override {
    int32 oldWidth = _width;
    int32 oldHeight = _height;
    MainWindow::WindowSizeCallback(w, h);

    if (oldWidth != _width || oldHeight != _height) {
      if (oldWidth == 0 || _width == 0 || oldHeight == 0 || _height == 0)
        return;

      float32 scaleX = (float32)_width / (float32)oldWidth;
      float32 scaleY = (float32)_height / (float32)oldHeight;

      if (_viewCount > 0) {
        for (int i = 0; i < MAX_VIEWPORT_COUNT; i++) {
          if (_views[i] != nullptr) {
            _views[i]->Resize(scaleX, scaleY);
          }
        }
      }
    }
  }

  virtual void KeyCallBack(int32 key, int32 scannode, int32 action,
                           int32 mods) override {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
      KeyPress(key, scannode, action, mods);
    } else if (action == GLFW_RELEASE) {
      KeyRelease(key, scannode, action, mods);
    }
  }

protected:
  virtual void Initialise() override;
  virtual void Terminate() override;
  virtual void HandleEvents() override;

  virtual void MouseLeftButtonClick(glm::dvec2 pos) override {
    _leftButtonDown = true;
  }
  virtual void MouseLeftButtonRelease(glm::dvec2 pos) override {
    _leftButtonDown = false;
    _leftButtonDragmove = false;
  }
  virtual void MouseRightButtonClick(glm::dvec2 pos) override {
    _rightButtonDown = true;
  }
  virtual void MouseRightButtonRelease(glm::dvec2 pos) override {
    _rightButtonDown = false;
    _rightButtonDragmove = false;
  }
  virtual void MouseMiddleButtonClick(glm::dvec2 pos) override {
    _middleButtonDown = true;
  }
  virtual void MouseMiddleButtonRelease(glm::dvec2 pos) override {
    _middleButtonDown = false;
    _middleButtonDragmove = false;
  }

  virtual void MouseMove(glm::dvec2 pos) override {}
  virtual void MouseLeftButtonDragMove(glm::dvec2 pos) override {
    for (int i = 0; i < MAX_VIEWPORT_COUNT; i++) {
      if (_views[i] == nullptr)
        continue;

      _views[i]->HandleMouseLeftButtonDrag(_cursorPos, pos);
    }

    _leftButtonDragmove = true;
  }
  virtual void MouseRightButtonDragMove(glm::dvec2 pos) override {
    for (int i = 0; i < MAX_VIEWPORT_COUNT; i++) {
      if (_views[i] == nullptr)
        continue;

      _views[i]->HandleMouseRightButtonDrag(_cursorPos, pos);
    }

    _rightButtonDragmove = true;
  }
  virtual void MouseMiddleButtonDragMove(glm::dvec2 pos) override {

    _middleButtonDragmove = true;
  }

  virtual void MouseScroll(glm::dvec2 scroll) override {
    for (int i = 0; i < MAX_VIEWPORT_COUNT; i++) {
      if (_views[i] == nullptr)
        continue;

      _views[i]->HandleMouseScroll(scroll);
    }
  }
  virtual void MouseDoubleClick() override {}

  virtual void KeyPress(int32 key, int32 scannode, int32 action,
                        int32 mods) override {
    for (int i = 0; i < MAX_VIEWPORT_COUNT; i++) {
      if (_views[i] == nullptr)
        continue;

      _views[i]->HandleKeyPress(key, scannode, action, mods);
    }
  }

  virtual void KeyRelease(int32 key, int32 scannode, int32 action,
                          int32 mods) override {
    for (int i = 0; i < MAX_VIEWPORT_COUNT; i++) {
      if (_views[i] == nullptr)
        continue;

      _views[i]->HandleKeyRelease(key, scannode, action, mods);
    }
  }

private:
private:
  std::vector<IMGUI_FUNC> _imguiFuncs;
  GLFWwindow *_window = nullptr;
  ImGuiContext *_imguiContext = nullptr;
};
} // namespace TwinkleGraphics

#endif