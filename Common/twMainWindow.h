

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

    virtual void Run() = 0;

protected:
    virtual void Initialise() = 0;
    virtual void Terminate() = 0;
    virtual void HandleEvents() = 0;

protected:
    View* _views[MAX_VIEWPORT_COUNT];

    int32 _width;
    int32 _height;

    int8 _view_count;
};

class GLFWMainWindow : public MainWindow
{
public:
    GLFWMainWindow(int32 width = 800, int32 height = 640);
    virtual ~GLFWMainWindow();

    void AddGUIFunc(IMGUI_FUNC func);
    virtual void Run() override;

protected:
    virtual void Initialise() override;
    virtual void Terminate() override;
    virtual void HandleEvents() override;


private:
    std::vector<IMGUI_FUNC>  _imgui_funcs;
    GLFWwindow* _window;
};
} // namespace TwinkleGraphics

#endif