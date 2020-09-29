#ifndef TW_WINDOW_H
#define TW_WINDOW_H

#include "twWidget.h"

namespace TwinkleGraphics
{
    class Window : public Widget
    {
    public:
        explicit Window(Widget* parent = nullptr);
        virtual ~Window();

    protected:
    };
} // namespace TwinkleGraphics

#endif