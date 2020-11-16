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

        virtual void Update(float deltaTime) override;
        virtual void OnGuiBegin()  override;
        virtual void OnGui() override;
        virtual void OnGuiEnd()  override;

        RenderTexture::Ptr GetViewRT() { return _viewRT; }

    private:
        virtual void SetFocusedInternal() override;
        virtual void SetHoveredInternal() override;
        void CreateViewRT();
        void ResizeViewRT();
        void PaintViewGui(ImVec2 viewSize);

    private:
        RenderTexture::Ptr _viewRT = nullptr;
        vec2 _viewSize;
        bool _viewSizeDirty = false;
        bool _viewSizeInitialized = false;
    };
} // namespace TwinkleGraphics

#endif