
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "twMainWindow.h"
#include "twConsoleLog.h"
#include "twResourceManager.h"
#include "twTextureManager.h"
#include "twImageManager.h"
#include "twModelManager.h"

namespace TwinkleGraphics
{
MainWindow::MainWindow(int width, int height)
    : _views()
    , _width(width)
    , _height(height)
    , _viewCount(0)
    , _leftButtonDown(false)
    , _rightButtonDown(false)
    , _middleButtonDown(false)
    , _leftButtonDragmove(false)
    , _rightButtonDragmove(false)
    , _middleButtonDragmove(false)
    , _mouseMoving(false)
    , _cursorEnterd(false)
    , _windowResize(false)
{
}

MainWindow::~MainWindow()
{
    for(auto view : _views)
    { view = nullptr; }
}

void MainWindow::AddViews(View **views, int num)
{
    if(views != nullptr)
    {
        for(int i = 0; i < num; i++)
        {
            if(views[i] !=  nullptr)
            {
                AddView(views[i]);
            }
        }        
    }
}

void MainWindow::AddView(View *view)
{
    if(view == nullptr 
        || _viewCount == MAX_VIEWPORT_COUNT
    )
        return;

    view->Init(glm::ivec2(_width, _height));
    for(int i = 0; i < MAX_VIEWPORT_COUNT; i++)
    {
        if(_views[i] == nullptr)
        {
            _views[i] = view;
            ++_viewCount;
            return;
        }
    }
}

void MainWindow::RemoveViews(View **views, int num)
{
    if(views != nullptr)
    {
        for(int i = 0; i < num; i++)
        {
            if(views[i] != nullptr)
            {
                RemoveView(views[i]);
            }
        }
    }
}

void MainWindow::RemoveView(View* view)
{
    for (int i = 0; i < MAX_VIEWPORT_COUNT; i++)
    {
        if (_views[i] != nullptr && _views[i] == view)
        {
            _views[i] = nullptr;
            --_viewCount;
            return;
        }
    }
}








/*------------------------------GLFWMainWindow----------------------------- */


GLFWMainWindow::GLFWMainWindow(int32 width, int32 height)
    : MainWindow(width, height)
{
    Initialise();
}

GLFWMainWindow::~GLFWMainWindow()
{
    _imguiFuncs.clear();
    Terminate();
}

void GLFWMainWindow::AddGUIFunc(IMGUI_FUNC func)
{
    _imguiFuncs.push_back(func);
}


void GLFWMainWindow::Reset()
{
}

void GLFWMainWindow::Run()
{
    ShaderManager& shaderMgr = ShaderMgrInstance();
    TextureManager& textureMgr = TextureMgrInstance();
    ImageManager& imgMgr = ImageMgrInstance();
    ModelManager& modelMgr = ModelMgrInstance();
    ResourceManager& resMgr = ResourceMgrInstance();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(_window))
    {
        resMgr.Update();
        shaderMgr.Update();
        imgMgr.Update();
        textureMgr.Update();
        modelMgr.Update();
        HandleEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        for(int i = 0, n = _imguiFuncs.size(); i < n; i++)
        {
            if(_imguiFuncs[i] != nullptr)
                _imguiFuncs[i]();
        }

        if(_viewCount > 0)
        {
            for(int i = 0; i < MAX_VIEWPORT_COUNT; i++)
            {
                if(_views[i] != nullptr)
                    _views[i]->OnViewGUI();
            }

            ImGui::Render();
            for(int i = 0; i < _viewCount; i++)
            {
                if(_views[i] != nullptr)
                    _views[i]->Run();
            }
        }
        else
        {
            ImGui::Render();
            glViewport(0, 0, _width, _height);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glClearColor(0.f, 0.0f, 0.0f, 1.0f);
        }

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(_window);
    }
}

void GLFWMainWindow::Initialise()
{
    /* Initialize the library */
    if (!glfwInit())
        return;

    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    
    /* Create a windowed mode window and its OpenGL context */
    _window = glfwCreateWindow(_width, _height, "Twinkle Shading", NULL, NULL);
    if (!_window)
    {
        glfwTerminate();
        return;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(_window);

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
    _imguiContext = ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto-Medium.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("Assets/Fonts/Cousine-Regular.ttf", 15.0f);
    // io.Fonts->AddFontFromFileTTF("Assets/Fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("Assets/Fonts/ProggyTiny.ttf", 10.0f);
    ImFont* font = io.Fonts->AddFontFromFileTTF("Assets/Fonts/simhei.ttf", 14.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
    IM_ASSERT(font != NULL);
    ImGui::GetIO().FontDefault = font;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

#ifdef _DEBUG
    char *vendor = (char *)glGetString(GL_VENDOR);
    char *renderer = (char *)glGetString(GL_RENDERER);
    char *version = (char *)glGetString(GL_VERSION);

    int count;
    glGetIntegerv(GL_NUM_EXTENSIONS, &count);
    for(int i = 0; i < count; i++)
    {
        const GLubyte* bytes = glGetStringi(GL_EXTENSIONS, i);
        GLExtensions.push_back(std::string(reinterpret_cast<const char*>(bytes)));
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
}

void GLFWMainWindow::Terminate()
{
    ResourceManager& resMgr = ResourceMgrInstance();
    resMgr.Destroy();
    
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(_imguiContext);

    glfwDestroyWindow(_window);
    glfwTerminate();
    _window = nullptr;
}

void GLFWMainWindow::HandleEvents()
{
    /* Poll for and process events */
    glfwPollEvents();
}

} // namespace TwinkleGraphics