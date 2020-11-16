#include "twGLFWViewWindow.h"

namespace TwinkleGraphics
{
    GLFWViewWindow::GLFWViewWindow(const std::string &name, uint32 width, uint32 height, Widget *parent)
        : Window(name, width, height, parent)
    {
        CreateViewRT();
    }

    GLFWViewWindow::~GLFWViewWindow()
    {
    }

    void GLFWViewWindow::OnGuiBegin()
    {
        assert(_viewRT != nullptr);
        ImGui::Begin(_windowname.c_str());
    }

    void GLFWViewWindow::OnGuiEnd()
    {
        SetFocusedInternal();
        SetHoveredInternal();

        ImGui::End();
    }

    void GLFWViewWindow::OnGui()
    {
        // ImVec2 size = ImGui::GetWindowSize();
        ImVec2 min = ImGui::GetWindowContentRegionMin();
        ImVec2 max = ImGui::GetWindowContentRegionMax();
        ImVec2 size(max.x - min.x, max.y - min.y);
        
        if(size.x == 0.0f) size.x = _data->width;
        if(size.y == 0.0f) size.y = _data->height;
        ImGui::BeginChild(u8"SubWindow", ImVec2(size.x, size.y), false, ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar
                          // | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove
        );

        {
            Texture::Ptr tex = _viewRT->GetTexture();
            uint id = tex->GetRenderRes().id;
            ImGui::Image((ImTextureID)id, ImVec2(size.x, size.y), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
        }
        ImGui::EndChild();
    }

    void GLFWViewWindow::SetFocusedInternal()
    {
        bool focused = ImGui::IsWindowFocused(ImGuiHoveredFlags_RootAndChildWindows);
        if(_focused != focused)
        {
            _focused = focused;
            if(_focused)
            {
                // focusedin
            }
            else
            {
                // focusedout
            }
            // Console::LogInfo("ViewWindow focused:", _focused, "\n");
        }
    }

    void GLFWViewWindow::SetHoveredInternal()
    {
        bool hovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
        if(_hovered != hovered)
        {
            _hovered = hovered;
            if(_hovered)
            {
                
            }
            else
            {
                /* code */
            }
            // Console::LogInfo("ViewWindow hovered:", _hovered, "\n");
        }
    }

    void GLFWViewWindow::CreateViewRT()
    {
        if (_viewRT != nullptr)
            return;

        _viewRT = std::make_shared<RenderTexture>(_data->width, _data->height, GL_RGBA8, GL_RGBA, false, false, false, 1, false);
        assert(_viewRT != nullptr);

        _viewRT->Create(nullptr);
        _viewRT->GetTexture()->SetFilter<FilterParam::MIN_FILTER>(FilterMode::LINEAR);
        _viewRT->GetTexture()->SetFilter<FilterParam::MAG_FILTER>(FilterMode::LINEAR);
    }

} // namespace TwinkleGraphics
