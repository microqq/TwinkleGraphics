#ifndef TW_GLFWMAINFRAME_H
#define TW_GLFWMAINFRAME_H

#include "twMainFrame.h"

namespace TwinkleGraphics
{
    class GLFWMainFrame final : public MainFrame
    {
    public:
        explicit GLFWMainFrame(Widget *parent = nullptr);
        virtual ~GLFWMainFrame();

        virtual void Update() override;

    private:
    };
} // namespace TwinkleGraphics

#endif