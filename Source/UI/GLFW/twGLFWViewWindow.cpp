#include "twGLFWViewWindow.h"

namespace TwinkleGraphics
{
    GLFWViewWindow::GLFWViewWindow(const std::string &name, uint32 width, uint32 height, Widget *parent)
        : Window(name, width, height, parent)
    {
        float ratio = (float)height / (float)width;
        _viewSize.x = width;
        _viewSize.y = width * ratio;
        CreateViewRT();
    }

    GLFWViewWindow::~GLFWViewWindow()
    {
    }

    void GLFWViewWindow::Update(float deltaTime)
    {
        if(_viewSizeDirty)
        {
            ResizeViewRT();
            _viewSizeDirty = false;
        }
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

        if(!ImGui::IsWindowCollapsed())
        {
            ImVec2 viewSize = ImVec2(_viewSize.x, _viewSize.y);
            if(_viewSizeInitialized)
            {
                ImVec2 windowSize = ImGui::GetWindowSize();

                ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
                ImVec2 contentMax = ImGui::GetWindowContentRegionMax();
                ImVec2 contentSize(contentMax.x - contentMin.x, contentMax.y - contentMin.y);
                ImVec2 titleBarSize(windowSize.x - contentSize.x, windowSize.y - contentSize.y);

                if(viewSize.x != contentSize.x || viewSize.y != contentSize.y)
                {
                    viewSize = contentSize;
                    _viewSize.x = viewSize.x;
                    _viewSize.y = viewSize.y;

                    _viewSizeDirty = true;
                }
            }
            else
            {
                _viewSizeInitialized = true;
            }

            PaintViewGui(viewSize);
        }
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

    void GLFWViewWindow::ResizeViewRT()
    {
        if(_viewRT != nullptr)
        {
            _viewRT->Resize(_viewSize.x, _viewSize.y);
        }
    }

    void GLFWViewWindow::PaintViewGui(ImVec2 viewSize)
    {
        ImGui::BeginChild(u8"SubWindow"
            , viewSize
            , false
            , ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar
                          // | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove
        );

        {
            Texture::Ptr tex = _viewRT->GetTexture();
            uint id = tex->GetRenderRes().id;
            ImGui::Image((ImTextureID)id, viewSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
        }
        ImGui::EndChild();
    }

} // namespace TwinkleGraphics
