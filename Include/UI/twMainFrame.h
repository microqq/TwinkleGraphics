#ifndef TW_MAINFRAME_H
#define TW_MAINFRAME_H

#include "twWidget.h"

namespace TwinkleGraphics
{
    class MainFrame : public Widget
    {
    public:
        MainFrame(uint32 width, uint32 height, Widget *parent = nullptr);
        virtual ~MainFrame();
        virtual void Update(float deltaTime = 0.0f) override;

        virtual bool CheckClose() { return false; }
        virtual void PollInputEvents() {}

        virtual void BeginFrame() {}
        virtual void EndFrame() {}

    protected:
    };

} // namespace TwinkleGraphics

#endif