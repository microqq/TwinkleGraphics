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
        virtual void PollInputEvents() override;

        virtual void BeginFrame() override;
        virtual void EndFrame() override;
    
    private:
        virtual void MouseInputCallback(int32 button, int32 action, int32 mods) override;
        virtual void CursorPosCallback(float64 xpos, float64 ypos) override;
        virtual void CursorEnterPosCallback(int32 entered) override;
        virtual void ScrollCallback(float64 dx, float64 dy) override;
        virtual void WindowSizeCallback(int32 w, int32 h) override;
        // virtual void FrameBufferSizeCallback(int32 w, int32 h) override;
        virtual void KeyInputCallBack(int32 key, int32 scannode, int32 action, int32 mods) override;


    private:
        void SetInputEventCallbacks();

    private:
        GLFWwindow* _window = nullptr;
    };
} // namespace TwinkleGraphics

#endif