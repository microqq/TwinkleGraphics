#ifndef TW_VIEWWINDOW_H
#define TW_VIEWWINDOW_H

#include "imgui.h"
#include "twWindow.h"

namespace TwinkleGraphics
{
    class ViewWindow : public Window
    {
    public:
        explicit ViewWindow(Widget* parent = nullptr);
        virtual ~ViewWindow();

    private:
        uint32 _renderTarget = 0;
    };
} // namespace TwinkleGraphics

#endif