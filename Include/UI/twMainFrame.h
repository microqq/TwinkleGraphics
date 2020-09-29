#ifndef TW_MAINFRAME_H
#define TW_MAINFRAME_H

#include "twWidget.h"

namespace TwinkleGraphics
{
    class __TWCOMExport MainFrame : public Widget
    {
    public:
        MainFrame(Widget *parent = nullptr);
        virtual ~MainFrame();
        virtual void Update() override;

    protected:
    };

} // namespace TwinkleGraphics

#endif