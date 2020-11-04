#include "twWindow.h"

namespace TwinkleGraphics
{
    Window::Window(const std::string& name, uint32 width, uint32 height, Widget* parent)
        : Widget(parent)
        , _windowname(name)
    {
        this->SetSize(width, height);
    }

    Window::~Window()
    {}    
} // namespace TwinkleGraphics
