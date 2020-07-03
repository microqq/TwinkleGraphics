

#ifndef TW_MainWINDOW_H
#define TW_MainWINDOW_H

#include <iostream>
#include <vector>

#include "twView.h"

#include <glfw/glfw3.h>

namespace TwinkleGraphics
{

class MainWindow
{
public:
    MainWindow(int32 width = 800, int32 height = 640);
    virtual ~MainWindow();

    void AddViews(View** views, int num);
    void AddView(View* view);
    void RemoveViews(View** view, int num);
    void RemoveView(View* view);
    void ResetViewPosSize(View* view, glm::ivec2 pos, glm::ivec2 size)
    {
        if(view == nullptr)
            return;
        view->ResetViewport(Rect(pos.x, pos.y, size.x, size.y));
    }
    void ResetViewPosSize(int32 index, glm::ivec2 pos, glm::ivec2 size)
    {
        if(index < 0 || index >= MAX_VIEWPORT_COUNT || _views[index] == nullptr)
            return;
        ResetViewPosSize(_views[index], pos, size);
    }

    virtual void Reset() = 0;
    virtual void Run() = 0;

    virtual void MouseButtonInput(int32 button, int32 action, int32 mods) {}
    virtual void CursorPosCallback(float64 xpos, float64 ypos) {}
    virtual void CursorEnterPosCallback(int32 entered) {}
    virtual void ScrollCallback(float64 dx, float64 dy) {}
    virtual void WindowSizeCallback(int32 w, int32 h) 
    {
        if(_width != w || _height == h)
        {
            _window_resize = true;
            _width = w; _height = h;
        }
    }

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

protected:
    View* _views[MAX_VIEWPORT_COUNT];

    glm::dvec2 _cursor_enter_pos;
    glm::dvec2 _cursor_pos;
 
    int32 _width;
    int32 _height;
    int8 _view_count;

    bool _left_button_down;
    bool _right_button_down;
    bool _middle_button_down;
    bool _left_button_dragmove;
    bool _right_button_dragmove;
    bool _middle_button_dragmove;
    bool _mouse_moving;
    bool _cursor_enterd;
    bool _window_resize;
};


class GLFWMainWindow : public MainWindow
{
public:
    GLFWMainWindow(int32 width = 800, int32 height = 640);
    virtual ~GLFWMainWindow();

    void AddGUIFunc(IMGUI_FUNC func);
    void SetMouseButtonInputCallback(GLFWmousebuttonfun func) { glfwSetMouseButtonCallback(_window, func); }
    void SetCursorPosCallback(GLFWcursorposfun func) { glfwSetCursorPosCallback(_window, func); }
    void SetCursorPosEnterCallback(GLFWcursorenterfun func) { glfwSetCursorEnterCallback(_window, func); }
    void SetScrollCallback(GLFWscrollfun func) { glfwSetScrollCallback(_window, func); }
    void SetWindowSizeCallback(GLFWwindowsizefun func) { glfwSetWindowSizeCallback(_window, func); }

    virtual void Reset() override;
    virtual void Run() override;

    virtual void MouseButtonInput(int32 button, int32 action, int32 mods) override
    {
        glfwGetCursorPos(_window, &(_cursor_pos.x), &(_cursor_pos.y));

        if(button == GLFW_MOUSE_BUTTON_LEFT)
        {
            if(action == GLFW_PRESS)
            {
                MouseLeftButtonClick(_cursor_pos);
            }
            else if(action == GLFW_RELEASE)
            {
                MouseLeftButtonRelease(_cursor_pos);
            }
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            if (action == GLFW_PRESS)
            {
                MouseRightButtonClick(_cursor_pos);
            }
            else if (action == GLFW_RELEASE)
            {
                MouseRightButtonRelease(_cursor_pos);
            }
        }
        else if(button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
            if (action == GLFW_PRESS)
            {
                MouseMiddleButtonClick(_cursor_pos);
            }
            else if (action == GLFW_RELEASE)
            {
                MouseMiddleButtonRelease(_cursor_pos);
            }
        }
    }
    virtual void CursorPosCallback(float64 xpos, float64 ypos) override
    {
        if(_left_button_down)
        {
            MouseLeftButtonDragMove(glm::dvec2(xpos, ypos));
        }
        else if(_right_button_down)
        {
            MouseRightButtonDragMove(glm::dvec2(xpos, ypos));
        }
        else if(_middle_button_down)
        {
            _middle_button_dragmove = true;
        }

        glfwGetCursorPos(_window, &(_cursor_pos.x), &(_cursor_pos.y));
    }

    virtual void CursorEnterPosCallback(int32 entered) override
    {
        if(entered)
        {
            _cursor_enterd = true;
            glfwGetCursorPos(_window, &(_cursor_enter_pos.x), &(_cursor_enter_pos.y));
        }
        else
        {
            _cursor_enterd = false;
        }
    }

    virtual void ScrollCallback(float64 dx, float64 dy) override
    {
        MouseScroll(glm::dvec2(dx, dy));
        return;
    }

    virtual void WindowSizeCallback(int32 w, int32 h) override
    {
        int32 old_width = _width;
        int32 old_height = _height;
        MainWindow::WindowSizeCallback(w, h);

        if (old_width != _width || old_height != _height)
        {
            if (old_width == 0 || _width == 0 ||
                old_height == 0 || _height == 0)
                return;

            float32 scale_x = (float32)_width / (float32)old_width;
            float32 scale_y = (float32)_height / (float32)old_height;

            if (_view_count > 0)
            {
                for (int i = 0; i < MAX_VIEWPORT_COUNT; i++)
                {
                    if (_views[i] != nullptr)
                    {
                        _views[i]->Resize(scale_x, scale_y);
                    }
                }
            }
        }
    }

protected:
    virtual void Initialise() override;
    virtual void Terminate() override;
    virtual void HandleEvents() override;

    virtual void MouseLeftButtonClick(glm::dvec2 pos) override
    {
        _left_button_down = true;
    }
    virtual void MouseLeftButtonRelease(glm::dvec2 pos) override
    {
        _left_button_down = false;
        _left_button_dragmove = false;
    }
    virtual void MouseRightButtonClick(glm::dvec2 pos) override
    {
        _right_button_down = true;
    }
    virtual void MouseRightButtonRelease(glm::dvec2 pos) override
    {
        _right_button_down = false;
        _right_button_dragmove = false;
    }
    virtual void MouseMiddleButtonClick(glm::dvec2 pos) override
    {
        _middle_button_down = true;
    }
    virtual void MouseMiddleButtonRelease(glm::dvec2 pos) override
    {
        _middle_button_down = false;
        _middle_button_dragmove = false;
    }


    virtual void MouseMove(glm::dvec2 pos) override {}
    virtual void MouseLeftButtonDragMove(glm::dvec2 pos) override 
    {
        for(int i = 0; i < MAX_VIEWPORT_COUNT; i++)
        {
            if(_views[i] == nullptr) continue;

            _views[i]->HandlerMouseLeftButtonDrag(_cursor_pos, pos);
        }

        _left_button_dragmove = true;
    }
    virtual void MouseRightButtonDragMove(glm::dvec2 pos) override 
    {
        for(int i = 0; i < MAX_VIEWPORT_COUNT; i++)
        {
            if(_views[i] == nullptr) continue;

            _views[i]->HandlerMouseRightButtonDrag(_cursor_pos, pos);
        }

        _right_button_dragmove = true;
    }
    virtual void MouseMiddleButtonDragMove(glm::dvec2 pos) override
    {


        _middle_button_dragmove = true;        
    }

    virtual void MouseScroll(glm::dvec2 scroll) override 
    {
        for(int i = 0; i < MAX_VIEWPORT_COUNT; i++)
        {
            if(_views[i] == nullptr) continue;

            _views[i]->HandlerMouseScroll(scroll);
        }
    }
    virtual void MouseDoubleClick() override {}


private:
    std::vector<IMGUI_FUNC>  _imgui_funcs;
    GLFWwindow* _window;
};
} // namespace TwinkleGraphics

#endif