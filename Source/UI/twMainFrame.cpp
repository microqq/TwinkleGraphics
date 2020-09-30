
#include "twApplication.h"
#include "twMainFrame.h"


namespace TwinkleGraphics
{
    MainFrame::MainFrame(uint32 width, uint32 height, Widget *parent)
        : Widget(parent)
    {
        SetSize(width, height);

        Application& app = ApplicationInstance();
        app._mainframe = this;
    }

    MainFrame::~MainFrame()
    {
    }

    void MainFrame::Update(float deltaTime)
    {
        Widget::Update(deltaTime);
    }
} // namespace TwinkleGraphics
