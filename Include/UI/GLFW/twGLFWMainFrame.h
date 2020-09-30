#ifndef TW_GLFWMAINFRAME_H
#define TW_GLFWMAINFRAME_H

#include <glew/glew.h>
#include "imgui.h"
#include <glfw/glfw3.h>

#include "twMainFrame.h"

namespace TwinkleGraphics
{
    class GLFWMainFrame : public MainFrame
    {
    public:
        explicit GLFWMainFrame(uint32 width, uint32 height, Widget *parent = nullptr);
        virtual ~GLFWMainFrame();

        virtual void Update(float deltaTime = 0.0f) override;
        virtual void Destroy() override;

        virtual bool CheckClose() override;
        virtual void DispatchInputEvents() override;

        virtual void BeginFrame() override;
        virtual void EndFrame() override;

    private:
        GLFWwindow* _window = nullptr;
    };
} // namespace TwinkleGraphics

#endif