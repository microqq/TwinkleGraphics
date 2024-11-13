
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "twConsoleLog.h"
#include "twImageManager.h"
#include "twMainWindow.h"
#include "twModelManager.h"
#include "twResourceManager.h"
#include "twTextureManager.h"
#include "twImGuiContextManager.h"


namespace TwinkleGraphics {
MainWindow::MainWindow(int width, int height)
    : _views(), _width(width), _height(height), _viewCount(0),
      _leftButtonDown(false), _rightButtonDown(false), _middleButtonDown(false),
      _leftButtonDragmove(false), _rightButtonDragmove(false),
      _middleButtonDragmove(false), _mouseMoving(false), _cursorEnterd(false),
      _windowResize(false) {}

MainWindow::~MainWindow() {}

void MainWindow::AddViews(View **views, int num) {
  if (views != nullptr) {
    for (int i = 0; i < num; i++) {
      if (views[i] != nullptr) {
        AddView(views[i]);
      }
    }
  }
}

void MainWindow::AddView(View *view) {
  if (view == nullptr || _viewCount == MAX_VIEWPORT_COUNT)
    return;

  view->Initialise(glm::ivec2(_width, _height));
  for (int i = 0; i < MAX_VIEWPORT_COUNT; i++) {
    if (_views[i] == nullptr) {
      _views[i] = view;
      ++_viewCount;
      return;
    }
  }
}

void MainWindow::RemoveViews(View **views, int num) {
  if (views != nullptr) {
    for (int i = 0; i < num; i++) {
      if (views[i] != nullptr) {
        RemoveView(views[i]);
      }
    }
  }
}

void MainWindow::RemoveView(View *view) {
  for (int i = 0; i < MAX_VIEWPORT_COUNT; i++) {
    if (_views[i] != nullptr && _views[i] == view) {
      _views[i] = nullptr;
      --_viewCount;
      return;
    }
  }
}

/*------------------------------GLFWMainWindow----------------------------- */

GLFWMainWindow::GLFWMainWindow(int32 width, int32 height)
    : MainWindow(width, height) {
}

GLFWMainWindow::~GLFWMainWindow() {
  _imguiFuncs.clear();
}

void GLFWMainWindow::AddGUIFunc(IMGUI_FUNC func) {
  _imguiFuncs.emplace_back(func);
}

void GLFWMainWindow::Reset() {}

void GLFWMainWindow::Run() {
  ShaderManager &shaderMgr = ShaderMgrInstance();
  TextureManager &textureMgr = TextureMgrInstance();
  ImageManager &imgMgr = ImageMgrInstance();
  ModelManager &modelMgr = ModelMgrInstance();
  ResourceManager &resMgr = ResourceMgrInstance();

  ImGuiIO &io = ImGui::GetIO();
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(_window)) {
    resMgr.Update();
    shaderMgr.Update();
    imgMgr.Update();
    textureMgr.Update();
    modelMgr.Update();
    HandleEvents();

    // maintain active, prevent user change via imgui dialog
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    for (int i = 0, n = _imguiFuncs.size(); i < n; i++) {
      if (_imguiFuncs[i] != nullptr)
        _imguiFuncs[i]();
    }

    if (_viewCount > 0) {
      for (int i = 0; i < MAX_VIEWPORT_COUNT; i++) {
        if (_views[i] != nullptr)
          _views[i]->OnViewGUI();
      }
    }
    ImGui::Render();

    int width, height = 0;
    glfwGetFramebufferSize(_window, &width, &height);

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(0.f, 0.0f, 0.0f, 1.0f);

    if (_viewCount > 0) {
      for (int i = 0; i < _viewCount; i++) {
        if (_views[i] != nullptr)
          _views[i]->Run();
      }
    }

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      GLFWwindow *backupContext = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backupContext);
    }
    glfwSwapBuffers(_window);
  }
    Terminate();
}

void GLFWMainWindow::InitWindow() {
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
  _window = glfwCreateWindow(_width, _height, "Twinkle Shading", NULL, NULL);
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
    // fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

    Console::LogError("OpenGL initialise ", glewGetErrorString(err), "\n");
  }
  // fprintf(stdout, "Status:Using GLEW %s\n", glewGetString(GLEW_VERSION));
  Console::LogInfo("OpenGL using GLEW-", glewGetString(GLEW_VERSION), "\n");
}


void GLFWMainWindow::InitOther() {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGuiContextManager &imguiCtxMgr = ImGuiContextMgrInstance();
  imguiCtxMgr.SetCurrentContext();
  // _imguiContext = ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  // io.ConfigFlags |=
  //     ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad
  // Controls
  (void)io;

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(_window, true);
  ImGui_ImplOpenGL3_Init("#version 410");

  io.Fonts->AddFontDefault();
  // io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto-Medium.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("Assets/Fonts/Cousine-Regular.ttf", 15.0f);
  // io.Fonts->AddFontFromFileTTF("Assets/Fonts/DroidSans.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("Assets/Fonts/ProggyTiny.ttf", 10.0f);
  ImFont *font = io.Fonts->AddFontFromFileTTF(
      "Assets/Fonts/Nowar-Neo-Sans-CN-Regular.ttf", 16.0f, NULL,
      io.Fonts->GetGlyphRangesChineseFull());
  IM_ASSERT(font != NULL);
  ImGui::GetIO().FontDefault = font;

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
    // style.Colors[ImGuiCol_Column]               = ImVec4(0.39f, 0.39f,
    // 0.39f, 1.00f); style.Colors[ImGuiCol_ColumnHovered]        = ImVec4(0.16f,
    // 0.62f, 0.87f, 0.78f); style.Colors[ImGuiCol_ColumnActive]         =
    // ImVec4(0.16f, 0.62f, 0.87f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
    style.Colors[ImGuiCol_ResizeGripHovered] =
        ImVec4(0.16f, 0.62f, 0.87f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive] =
        ImVec4(0.16f, 0.62f, 0.87f, 0.95f);
    // style.Colors[ImGuiCol_CloseButton]          = ImVec4(0.59f, 0.59f, 0.59f,
    // 0.50f); style.Colors[ImGuiCol_CloseButtonHovered]   = ImVec4(0.98f, 0.39f,
    // 0.36f, 1.00f); style.Colors[ImGuiCol_CloseButtonActive]    = ImVec4(0.98f,
    // 0.39f, 0.36f, 1.00f);
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
  for (int i = 0; i < count; i++) {
    const GLubyte *bytes = glGetStringi(GL_EXTENSIONS, i);
    GLExtensions.emplace_back(std::string(reinterpret_cast<const char *>(bytes)));
  }

  Console::LogInfo("Vendor:=" + std::string(vendor) + "\n");
  Console::LogInfo("Render:=", renderer, "\n");
  Console::LogInfo("OpenGL Version:=", version, "\n");

  int attribCounts;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attribCounts);
  Console::LogInfo("GL_MAX_VERTEX_ATTRIBS:=", attribCounts, "\n");

  int32 texunitCount;
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texunitCount);
  Console::LogInfo("GL_MAX_TEXTURE_IMAGE_UNITS:=", texunitCount, "\n");

  int32 combinedtexunitCount;
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &combinedtexunitCount);
  Console::LogInfo(
      "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:=", combinedtexunitCount, "\n");

  int32 max_colorattachmentCount;
  glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &max_colorattachmentCount);
  Console::LogInfo("GL_MAX_COLOR_ATTACHMENTS:=", max_colorattachmentCount,
                   "\n");

  int32 maxDrawbuffersCount;
  glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawbuffersCount);
  Console::LogInfo("GL_MAX_DRAW_BUFFERS:=", maxDrawbuffersCount, "\n");
#endif
}

void GLFWMainWindow::Terminate() {
  ResourceManager &resMgr = ResourceMgrInstance();
  resMgr.Destroy();

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext(_imguiContext);

  glfwDestroyWindow(_window);
  glfwTerminate();
  _window = nullptr;
}

void GLFWMainWindow::HandleEvents() {
  /* Poll for and process events */
  glfwPollEvents();
}

} // namespace TwinkleGraphics
