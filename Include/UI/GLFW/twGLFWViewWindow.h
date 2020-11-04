#ifndef TW_VIEWWINDOW_H
#define TW_VIEWWINDOW_H

#include "imgui.h"
#include "twWindow.h"
#include "twRenderTexture.h"

namespace TwinkleGraphics
{
    class GLFWViewWindow : public Window
    {
    public:
        explicit GLFWViewWindow(const std::string& name, uint32 width, uint32 height, Widget* parent = nullptr);
        virtual ~GLFWViewWindow();

        virtual void OnGui() override;

        RenderTexture::Ptr GetViewRT() { return _viewRT; }

    private:
        void CreateViewRT();

    private:
        RenderTexture::Ptr _viewRT = nullptr;
    };
} // namespace TwinkleGraphics

#endif