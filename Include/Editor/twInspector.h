#ifndef TW_INSPECTOR_H
#define TW_INSPECTOR_H

#include "twWidget.h"

namespace TwinkleGraphics
{
    class Inspector : public Widget
    {
    public:
        explicit Inspector(Widget *parent = nullptr);
        virtual ~Inspector();

        virtual void OnGui() override;

    private:
    };
} // namespace TwinkleGraphics

#endif