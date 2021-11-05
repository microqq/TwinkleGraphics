
#include "twMainFrame.h"
#include "twApplication.h"

namespace TwinkleGraphics {
MainFrame::MainFrame(uint32 width, uint32 height, Widget *parent)
    : Widget(parent) {
  SetSize(width, height);
  SetDepth(0);

  Application &app = ApplicationInstance();
  app._mainframe = this;
}

MainFrame::~MainFrame() {}

void MainFrame::Update(float deltaTime) { Widget::Update(deltaTime); }
} // namespace TwinkleGraphics
