

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

    virtual void Reset() = 0;
    virtual void Run() = 0;

    virtual void MouseButtonInput(int32 button, int32 action, int32 mods) {}
    virtual void CursorPosCallback(float64 xpos, float64 ypos) {}
    virtual void CursorEnterPosCallback(int32 entered) {}

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
    virtual void MouseMove(glm::dvec2 move) = 0;
    virtual void MouseLeftButtonDragMove(glm::dvec2 move) = 0;
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

    virtual void Reset() override;
    virtual void Run() override;

    virtual void MouseButtonInput(int32 button, int32 action, int32 mods) override
    {
        glfwGetCursorPos(_window, &(_cursor_pos.x), &(_cursor_pos.y));

        std::cout << "mouse event x pos:" << _cursor_pos.x << "mouse event y pos:" << _cursor_pos.y << std::endl;

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
        std::cout << "x pos:" << xpos << " y pos:" << ypos << std::endl;

        if(_left_button_down)
        {
            MouseLeftButtonDragMove(glm::dvec2(xpos, ypos));
        }
        else if(_right_button_down)
        {
            _right_button_dragmove = true;
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
    }
    virtual void MouseRightButtonClick(glm::dvec2 pos) override
    {
        _right_button_down = true;
    }
    virtual void MouseRightButtonRelease(glm::dvec2 pos) override
    {
        _right_button_down = false;
    }
    virtual void MouseMiddleButtonClick(glm::dvec2 pos) override
    {
        _middle_button_down = true;
    }
    virtual void MouseMiddleButtonRelease(glm::dvec2 pos) override
    {
        _middle_button_down = false;
    }


    virtual void MouseMove(glm::dvec2 move) override {}
    virtual void MouseLeftButtonDragMove(glm::dvec2 move) override 
    {
        move = move - _cursor_pos;

        for(int i = 0; i < MAX_VIEWPORT_COUNT; i++)
        {
            if(_views[i] == nullptr) continue;

            _views[i]->HandlerMouseLeftButtonDrag(move);
        }

        _left_button_dragmove = true;
    }
    virtual void MouseScroll(glm::dvec2 scroll) override {}
    virtual void MouseDoubleClick() override {}


private:
    std::vector<IMGUI_FUNC>  _imgui_funcs;
    GLFWwindow* _window;
};
} // namespace TwinkleGraphics

#endif