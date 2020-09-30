#ifndef TW_VIEWWINDOW_H
#define TW_VIEWWINDOW_H

#include "imgui.h"
#include "twWindow.h"

namespace TwinkleGraphics
{
    class GLFWViewWindow : public Window
    {
    public:
        explicit GLFWViewWindow(Widget* parent = nullptr);
        virtual ~GLFWViewWindow();

    private:
        uint32 _renderTarget = 0;
    };
} // namespace TwinkleGraphics

#endif