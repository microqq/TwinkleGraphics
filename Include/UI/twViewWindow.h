#ifndef TW_VIEWWINDOW_H
#define TW_VIEWWINDOW_H

#include "imgui.h"
#include "twCommon.h"

namespace TwinkleGraphics
{
    class ViewWindow
    {
    public:
        explicit ViewWindow();
        virtual ~ViewWindow();

    protected:

    private:
        uint32 _renderTarget = 0;
    };
} // namespace TwinkleGraphics

#endif