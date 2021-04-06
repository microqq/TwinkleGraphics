#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "twGLFWMainFrame.h"
#include "twConsoleLog.h"
#include "twLambdaTraits.h"
#include "twInput.h"

namespace TwinkleGraphics
{
    GLFWMainFrame::GLFWMainFrame(uint32 width, uint32 height, Widget *parent)
        : MainFrame(width, height, parent)
    {
        /* Initialize the library */
        if (!glfwInit())
            return;

        // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
        glfwWindowHint(GLFW_SAMPLES, 4);

        /* Create a windowed mode window and its OpenGL context */
        _window = glfwCreateWindow(_data->width, _data->height, "Twinkle Shading", NULL, NULL);
        if (!_window)
        {
            glfwTerminate();
            return;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(_window);
        glfwSwapInterval(1); // Enable vsync

        GLenum err = glewInit();
        if (GLEW_OK != err)
        {
            /* Problem: glewInit failed, something is seriously wrong. */
            // fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

            Console::LogError("Error:", glewGetErrorString(err), "\n");
        }
        // fprintf(stdout, "Status:Using GLEW %s\n", glewGetString(GLEW_VERSION));
        Console::LogInfo("Status:Using GLEW-", glewGetString(GLEW_VERSION), "\n");

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
        (void)io;

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(_window, true);
        ImGui_ImplOpenGL3_Init("#version 410");

        io.Fonts->AddFontDefault();
        // io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto-Medium.ttf", 16.0f);
        // io.Fonts->AddFontFromFileTTF("Assets/Fonts/Cousine-Regular.ttf", 15.0f);
        // io.Fonts->AddFontFromFileTTF("Assets/Fonts/DroidSans.ttf", 16.0f);
        // io.Fonts->AddFontFromFileTTF("Assets/Fonts/ProggyTiny.ttf", 10.0f);
        ImFont *font = io.Fonts->AddFontFromFileTTF("Assets/Fonts/simhei.ttf", 14.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
        IM_ASSERT(font != NULL);
        ImGui::GetIO().FontDefault = font;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle &style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;

            // color scheme adapted from
            // https://github.com/ocornut/imgui/pull/511#issuecomment-175719267
            style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
            style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
            style.Colors[ImGuiCol_WindowBg] = ImVec4(0.90f, 0.90f, 0.90f, 0.70f);
            style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            style.Colors[ImGuiCol_PopupBg] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
            style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
            style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
            style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.16f, 0.62f, 0.87f, 0.40f);
            style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.62f, 0.87f, 0.67f);
            style.Colors[ImGuiCol_TitleBg] = ImVec4(0.16f, 0.62f, 0.87f, 0.80f);
            style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.62f, 0.87f, 0.80f);
            style.Colors[ImGuiCol_TitleBgCollapsed] =
                ImVec4(0.16f, 0.62f, 0.87f, 0.40f);
            style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
            style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
            style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
            style.Colors[ImGuiCol_ScrollbarGrabHovered] =
                ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
            style.Colors[ImGuiCol_ScrollbarGrabActive] =
                ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
            style.Colors[ImGuiCol_CheckMark] = ImVec4(0.16f, 0.62f, 0.87f, 1.00f);
            style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.16f, 0.62f, 0.87f, 0.78f);
            style.Colors[ImGuiCol_SliderGrabActive] =
                ImVec4(0.16f, 0.62f, 0.87f, 1.00f);
            style.Colors[ImGuiCol_Button] = ImVec4(0.16f, 0.62f, 0.87f, 0.40f);
            style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.16f, 0.62f, 0.87f, 1.00f);
            style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.16f, 0.62f, 0.87f, 1.00f);
            style.Colors[ImGuiCol_Header] = ImVec4(0.16f, 0.62f, 0.87f, 0.31f);
            style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.16f, 0.62f, 0.87f, 0.80f);
            style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.16f, 0.62f, 0.87f, 1.00f);
            //style.Colors[ImGuiCol_Column]               = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
            //style.Colors[ImGuiCol_ColumnHovered]        = ImVec4(0.16f, 0.62f, 0.87f, 0.78f);
            //style.Colors[ImGuiCol_ColumnActive]         = ImVec4(0.16f, 0.62f, 0.87f, 1.00f);
            style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
            style.Colors[ImGuiCol_ResizeGripHovered] =
                ImVec4(0.16f, 0.62f, 0.87f, 0.67f);
            style.Colors[ImGuiCol_ResizeGripActive] =
                ImVec4(0.16f, 0.62f, 0.87f, 0.95f);
            //style.Colors[ImGuiCol_CloseButton]          = ImVec4(0.59f, 0.59f, 0.59f, 0.50f);
            //style.Colors[ImGuiCol_CloseButtonHovered]   = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
            //style.Colors[ImGuiCol_CloseButtonActive]    = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
            style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
            style.Colors[ImGuiCol_PlotLinesHovered] =
                ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
            style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
            style.Colors[ImGuiCol_PlotHistogramHovered] =
                ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
            style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.16f, 0.62f, 0.87f, 0.35f);
            style.Colors[ImGuiCol_ModalWindowDimBg] =
                ImVec4(0.20f, 0.20f, 0.20f, 0.70f);
        }        

#ifdef _DEBUG
        char *vendor = (char *)glGetString(GL_VENDOR);
        char *renderer = (char *)glGetString(GL_RENDERER);
        char *version = (char *)glGetString(GL_VERSION);

        int count;
        glGetIntegerv(GL_NUM_EXTENSIONS, &count);
        for (int i = 0; i < count; i++)
        {
            const GLubyte *bytes = glGetStringi(GL_EXTENSIONS, i);
            GLExtensions.push_back(std::string(reinterpret_cast<const char *>(bytes)));
        }

        Console::LogInfo("Vendor:" + std::string(vendor) + "\n");
        Console::LogInfo("Render:", renderer, "\n");
        Console::LogInfo("OGL Version:", version, "\n");

        int attrib_counts;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attrib_counts);
        Console::LogInfo("GL_MAX_VERTEX_ATTRIBS:", attrib_counts, "\n");

        int32 texunit_count;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texunit_count);
        Console::LogInfo("GL_MAX_TEXTURE_IMAGE_UNITS:", texunit_count, "\n");

        int32 combinedtexunit_count;
        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &combinedtexunit_count);
        Console::LogInfo("GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:", combinedtexunit_count, "\n");

        int32 max_colorattachment_count;
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &max_colorattachment_count);
        Console::LogInfo("GL_MAX_COLOR_ATTACHMENTS:", max_colorattachment_count, "\n");

        int32 max_drawbuffers_count;
        glGetIntegerv(GL_MAX_DRAW_BUFFERS, &max_drawbuffers_count);
        Console::LogInfo("GL_MAX_DRAW_BUFFERS:", max_drawbuffers_count, "\n");
#endif

        SetInputEventCallbacks();
    }

    GLFWMainFrame::~GLFWMainFrame()
    {
        Destroy();
    }

    void GLFWMainFrame::Update(float deltaTime)
    {
        MainFrame::Update(deltaTime);
    }

    void GLFWMainFrame::Destroy()
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(_window);
        glfwTerminate();
        _window = nullptr;

        MainFrame::Destroy();
    }

    bool GLFWMainFrame::CheckClose()
    {
        return glfwWindowShouldClose(_window);
    }

    void GLFWMainFrame::PollInputEvents()
    {
        glfwPollEvents();
    }

    void GLFWMainFrame::BeginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glViewport(_data->x, _data->y, _data->width, _data->height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glClearColor(0.4f, 0.2f, 0.0f, 1.0f);
    }

    void GLFWMainFrame::EndFrame()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO &io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(_window);
    }

    void GLFWMainFrame::MouseInputCallback(int32 button, int32 action, int32 mods)
    {
        dvec2 cursorPos;
        glfwGetCursorPos(_window, &(cursorPos.x), &(cursorPos.y));

        InputManager& inputMgrInst = InputMgrInstance();

        inputMgrInst.SetPressedMouse((MouseButton)button, (MouseState)action);
        inputMgrInst.SetMousePosition(cursorPos);
    }

    void GLFWMainFrame::CursorPosCallback(float64 xpos, float64 ypos)
    {
        InputManager& inputMgrInst = InputMgrInstance();
        dvec2 oldCursPos = inputMgrInst.GetMousePosition();
        dvec2 cursorPos;
        glfwGetCursorPos(_window, &(cursorPos.x), &(cursorPos.y));

        if(oldCursPos != cursorPos)
        {
            inputMgrInst.SetMouseMove(cursorPos);  
            // Console::LogInfo("Mouse motion x:", cursorPos.x, " y:", cursorPos.y, "\n");
        }
    }

    void GLFWMainFrame::CursorEnterPosCallback(int32 entered)
    {
        dvec2 cursorPos;
        glfwGetCursorPos(_window, &(cursorPos.x), &(cursorPos.y));

        InputManager& inputMgrInst = InputMgrInstance();
        inputMgrInst.SetCursorEnter(cursorPos, entered);
    }

    void GLFWMainFrame::ScrollCallback(float64 dx, float64 dy)
    {
        InputManager& inputMgrInst = InputMgrInstance();
        inputMgrInst.SetScroll(dx, dy);
    }

    void GLFWMainFrame::WindowSizeCallback(int32 w, int32 h)
    {
        InputManager& inputMgrInst = InputMgrInstance();
        inputMgrInst.SetWindowResize(vec2(w, h));

        _data->width = w;
        _data->height = h;
    }

    void GLFWMainFrame::KeyInputCallBack(int32 key, int32 scannode, int32 action, int32 mods)
    {
        InputManager& inputMgrInst = InputMgrInstance();
        inputMgrInst.SetPressedKey((KeyCode)key, (KeyState)action);
    }

    void GLFWMainFrame::SetInputEventCallbacks()
    {
        // Mouse Input
        GLFWmousebuttonfun mouseInputCallback(cify([this](GLFWwindow *window, int32 button, int32 action, int32 mods) {
            MouseInputCallback(button, action, mods);

            // Console::LogInfo("Mouse Input.");
        }));
        glfwSetMouseButtonCallback(_window, mouseInputCallback);

        // Cursor Input
        GLFWcursorposfun cursorInputCallback(cify([this](GLFWwindow *window, double x, double y) {
            CursorPosCallback(x, y);

            // Console::LogInfo("Cursor Input.");
        }));
        glfwSetCursorPosCallback(_window, cursorInputCallback);

        // CursorEnter Input
        GLFWcursorenterfun cursorenterInputCallback(cify([this](GLFWwindow *window, int entered) {
            CursorEnterPosCallback(entered);

            // Console::LogInfo("Cursor Enter Input.");
        }));
        glfwSetCursorEnterCallback(_window, cursorenterInputCallback);

        // Scroll Input
        GLFWscrollfun scrollInputCallback(cify([this](GLFWwindow *window, double x, double y) {
            ScrollCallback(x, y);

            // Console::LogInfo("Scroll Input.");
        }));
        glfwSetScrollCallback(_window, scrollInputCallback);

        // WindowSize Input
        GLFWwindowsizefun windowSizeCallback(cify([this](GLFWwindow *window, int w, int h) {
            WindowSizeCallback(w, h);

            // Console::LogInfo("WindowSize Input.");
        }));
        glfwSetWindowSizeCallback(_window, windowSizeCallback);

        // Key Input
        GLFWkeyfun keyCallback(cify([this](GLFWwindow *window, int32 key, int32 scannode, int32 action, int32 mods) {
            KeyInputCallBack(key, scannode, action, mods);

            // Console::LogInfo("Key Input.");
        }));
        glfwSetKeyCallback(_window, keyCallback);
    }

} // namespace TwinkleGraphics
