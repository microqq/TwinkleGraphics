#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "twConsoleLog.h"
#include "twGLFWMainFrame.h"
#include "twInput.h"
#include "twLambdaTraits.h"

#include "twCustomFont_1.cpp"
#include "twCustomFont_1.h"
#include "twDockSpaceLayout.h"

namespace TwinkleGraphics {
GLFWMainFrame::GLFWMainFrame(uint32 width, uint32 height, Widget *parent)
    : MainFrame(width, height, parent) {
  /* Initialize the library */
  if (!glfwInit())
    return;

#ifdef __APPLE__
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  /* Create a windowed mode window and its OpenGL context */
  _window = glfwCreateWindow(_pData->width, _pData->height, "Twinkle Shading",
                             NULL, NULL);
  if (!_window) {
    glfwTerminate();
    return;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(_window);
  glfwSwapInterval(1); // Enable vsync

  GLenum err = glewInit();
  if (GLEW_OK != err) {
    /* Problem: glewInit failed, something is seriously wrong. */
    Console::LogError("Opengl Initialise ", glewGetErrorString(err), "\n");
  }
  Console::LogInfo("OpenGL using GLEW-", glewGetString(GLEW_VERSION), "\n");

  // set user glfw callback
  // we should set custom glfw callback befor imgui init.
  // https://stackoverflow.com/questions/71680516/how-do-i-handle-mouse-events-in-general-in-imgui-with-glfw
  SetInputEventCallbacks();

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(_window, true);
  ImGui_ImplOpenGL3_Init("#version 410");

  io.Fonts->AddFontDefault();
  ImFont *font = io.Fonts->AddFontFromFileTTF(
      "Assets/Fonts/Nowar-Neo-Sans-CN-Regular.ttf", 16.0f, NULL,
      io.Fonts->GetGlyphRangesChineseFull());
  IM_ASSERT(font != NULL);
  ImGui::GetIO().FontDefault = font;

  // use icon font
  static const ImWchar icons_ranges[] = {ICON_MIN_IGFD, ICON_MAX_IGFD, 0};
  ImFontConfig icons_config;
  icons_config.MergeMode = true;
  icons_config.PixelSnapH = true;
  ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(
      FONT_ICON_BUFFER_NAME_IGFD, 15.0f, &icons_config, icons_ranges);

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform
  // windows can look identical to regular ones.
  ImGuiStyle &style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowBorderSize = 1;
    style.WindowRounding = 4;
    style.FrameRounding = 4;
    style.GrabMinSize = 10;
    style.GrabRounding = 4;

    // color scheme adapted from
    // https://github.com/GraphicsProgramming/dear-imgui-styles
  }

#ifdef _DEBUG
  char *vendor = (char *)glGetString(GL_VENDOR);
  char *renderer = (char *)glGetString(GL_RENDERER);
  char *version = (char *)glGetString(GL_VERSION);

  int count;
  glGetIntegerv(GL_NUM_EXTENSIONS, &count);
  for (int i = 0; i < count; i++) {
    const GLubyte *bytes = glGetStringi(GL_EXTENSIONS, i);
    GLExtensions.push_back(std::string(reinterpret_cast<const char *>(bytes)));
  }

  Console::LogInfo("Vendor:=" + std::string(vendor) + "\n");
  Console::LogInfo("Render:=", renderer, "\n");
  Console::LogInfo("OpenGL Version:=", version, "\n");

  int attrib_counts;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attrib_counts);
  Console::LogInfo("GL_MAX_VERTEX_ATTRIBS:=", attrib_counts, "\n");

  int32 texunit_count;
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texunit_count);
  Console::LogInfo("GL_MAX_TEXTURE_IMAGE_UNITS:=", texunit_count, "\n");

  int32 combinedtexunit_count;
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &combinedtexunit_count);
  Console::LogInfo(
      "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:=", combinedtexunit_count, "\n");

  int32 max_colorattachment_count;
  glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &max_colorattachment_count);
  Console::LogInfo("GL_MAX_COLOR_ATTACHMENTS:=", max_colorattachment_count,
                   "\n");

  int32 max_drawbuffers_count;
  glGetIntegerv(GL_MAX_DRAW_BUFFERS, &max_drawbuffers_count);
  Console::LogInfo("GL_MAX_DRAW_BUFFERS:=", max_drawbuffers_count, "\n");
#endif

  // set dockspace layout
  _pLayout = new DockSpaceLayout(this);
}

GLFWMainFrame::~GLFWMainFrame() {}

void GLFWMainFrame::Update(float deltaTime) { MainFrame::Update(deltaTime); }

void GLFWMainFrame::Destroy() {
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(_window);
  glfwTerminate();
  _window = nullptr;

  SAFE_DEL(_pLayout);
  _pLayout = nullptr;

  MainFrame::Destroy();
}

bool GLFWMainFrame::CheckIfClose() { return glfwWindowShouldClose(_window); }

void GLFWMainFrame::PollInputEvents() { glfwPollEvents(); }

void GLFWMainFrame::BeginFrame() {
  // maintain active, prevent user change via imgui dialog
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking

  int width, height = 0;
  glfwGetFramebufferSize(_window, &width, &height);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImVec2 pos, size;
  ImGuiViewport *viewport = ImGui::GetMainViewport();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    if (viewport) {
      pos = viewport->WorkPos;
      size = viewport->WorkSize;
    }
  } else {
    pos = ImVec2(0, 0);
    size = ImVec2((float)width, (float)height);
  }

  _pData->width = size.x;
  _pData->height = size.y;

  const auto dockSpaceLayout = dynamic_cast<DockSpaceLayout *>(_pLayout);
  dockSpaceLayout->SetImGuiViewport(viewport);

  glViewport(_pData->x, _pData->y, _pData->width, _pData->height);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glClearColor(0.10f, 0.10f, 0.10f, 1.0f);
}

void GLFWMainFrame::EndFrame() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  ImGuiIO &io = ImGui::GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow *backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }

  glfwSwapBuffers(_window);
}

void GLFWMainFrame::OnGuiBegin() {
  // layout begin
  _pLayout->Begin();
}

void GLFWMainFrame::OnGuiEnd() {
  // layout end
  _pLayout->End();
}

void GLFWMainFrame::MouseInputCallback(int32 button, int32 action, int32 mods) {
  dvec2 cursorPos;
  glfwGetCursorPos(_window, &(cursorPos.x), &(cursorPos.y));

  InputManager &inputMgrInst = InputMgrInstance();

  inputMgrInst.SetPressedMouse((MouseButton)button, (MouseState)action);
  inputMgrInst.SetMousePosition(cursorPos);
}

void GLFWMainFrame::CursorPosCallback(float64 xpos, float64 ypos) {
  InputManager &inputMgrInst = InputMgrInstance();
  dvec2 oldCursPos = inputMgrInst.GetMousePosition();
  dvec2 cursorPos;
  glfwGetCursorPos(_window, &(cursorPos.x), &(cursorPos.y));

  if (oldCursPos != cursorPos) {
    inputMgrInst.SetMouseMove(cursorPos);
    // Console::LogInfo("Mouse motion x:", cursorPos.x, " y:", cursorPos.y,
    // "\n");
  }
}

void GLFWMainFrame::CursorEnterPosCallback(int32 entered) {
  dvec2 cursorPos;
  glfwGetCursorPos(_window, &(cursorPos.x), &(cursorPos.y));

  InputManager &inputMgrInst = InputMgrInstance();
  inputMgrInst.SetCursorEnter(cursorPos, entered);
}

void GLFWMainFrame::ScrollCallback(float64 dx, float64 dy) {
  InputManager &inputMgrInst = InputMgrInstance();
  inputMgrInst.SetScroll(dx, dy);
}

void GLFWMainFrame::WindowSizeCallback(int32 w, int32 h) {
  InputManager &inputMgrInst = InputMgrInstance();
  inputMgrInst.SetWindowSize(vec2(w, h));

  _pData->width = w;
  _pData->height = h;
}

void GLFWMainFrame::KeyInputCallBack(int32 key, int32 scannode, int32 action,
                                     int32 mods) {
  InputManager &inputMgrInst = InputMgrInstance();
  inputMgrInst.SetPressedKey((KeyCode)key, (KeyState)action);
}

void GLFWMainFrame::SetInputEventCallbacks() {
  // Mouse Input
  GLFWmousebuttonfun mouseInputCallback(
      cify([this](GLFWwindow *window, int32 button, int32 action, int32 mods) {
        MouseInputCallback(button, action, mods);

        // Console::LogInfo("Mouse Input.");
      }));
  glfwSetMouseButtonCallback(_window, mouseInputCallback);

  // Cursor Input
  GLFWcursorposfun cursorInputCallback(
      cify([this](GLFWwindow *window, double x, double y) {
        CursorPosCallback(x, y);

        // Console::LogInfo("Cursor Input.");
      }));
  glfwSetCursorPosCallback(_window, cursorInputCallback);

  // CursorEnter Input
  GLFWcursorenterfun cursorenterInputCallback(
      cify([this](GLFWwindow *window, int entered) {
        CursorEnterPosCallback(entered);

        // Console::LogInfo("Cursor Enter Input.");
      }));
  glfwSetCursorEnterCallback(_window, cursorenterInputCallback);

  // Scroll Input
  GLFWscrollfun scrollInputCallback(
      cify([this](GLFWwindow *window, double x, double y) {
        ScrollCallback(x, y);

        // Console::LogInfo("Scroll Input.");
      }));
  glfwSetScrollCallback(_window, scrollInputCallback);

  // WindowSize Input
  GLFWwindowsizefun windowSizeCallback(
      cify([this](GLFWwindow *window, int w, int h) {
        WindowSizeCallback(w, h);

        // Console::LogInfo("WindowSize Input.");
      }));
  glfwSetWindowSizeCallback(_window, windowSizeCallback);

  // Key Input
  GLFWkeyfun keyCallback(cify([this](GLFWwindow *window, int32 key,
                                     int32 scannode, int32 action, int32 mods) {
    KeyInputCallBack(key, scannode, action, mods);

    // Console::LogInfo("Key Input.");
  }));
  glfwSetKeyCallback(_window, keyCallback);
}
} // namespace TwinkleGraphics
