#include "twWindow.h"

namespace TwinkleGraphics {
Window::Window(const std::string &name, uint32 width, uint32 height,
               Widget *parent)
    : Widget(parent, name) {
  this->SetSize(width, height);
}

Window::~Window() {}
} // namespace TwinkleGraphics
