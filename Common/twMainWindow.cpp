#include "twMainWindow.h"

namespace TwinkleGraphics
{
MainWindow::MainWindow(int width, int height)
    : _width(width)
    , _height(height)
{
    Initialise();
}

MainWindow::~MainWindow()
{
    Terminate();
}

void MainWindow::Frame()
{
    Advance(0.0f);

    Render();

    HandleEvents();
}

void MainWindow::SetViewport(int8 index, const Viewport& viewport)
{
    if(index >= MAX_VIEWPORT_COUNT)
    {
        return;
    }

    _viewports[_viewport_count++] = viewport;
}

void MainWindow::SetViewport(int8 index, const Rect& rect, uint32 mask, const RGBA& color, float32 depth, float32 stencil)
{
    if(index >= MAX_VIEWPORT_COUNT)
    {
        return;
    }

    Viewport& viewport = _viewports[_viewport_count++];
    viewport.rect = rect;
    viewport.clear_color = color;
    viewport.clear_depth = depth;
    viewport.clear_stencil = stencil;
    viewport.clear_mask = mask;
}

GLFWMainWindow::GLFWMainWindow(int32 width, int32 height)
    : MainWindow(width, height)
{}

GLFWMainWindow::~GLFWMainWindow()
{}

void GLFWMainWindow::Run()
{
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(_window))
    {
        Frame();
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

    /* Create a windowed mode window and its OpenGL context */
    _window = glfwCreateWindow(1024, 768, "Twinkle Shading", NULL, NULL);
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
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    char *vendor = (char *)glGetString( GL_VENDOR );
	char *renderer = (char *)glGetString( GL_RENDERER );
	char *version = (char *)glGetString( GL_VERSION );

    std::cout << "Vendor:" << vendor <<std::endl;
    std::cout << "Render:" << renderer <<std::endl;
    std::cout << "OGL Version:" << version <<std::endl;

    int attrib_counts;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attrib_counts);
    std::cout << "GL_MAX_VERTEX_ATTRIBS:" << attrib_counts << std::endl;    
}

void GLFWMainWindow::Terminate()
{
    glfwTerminate();
}

void GLFWMainWindow::Advance(float64 delta_time)
{

}

void GLFWMainWindow::Render()
{
    for(int i = 0; i < _viewport_count; i++)
    {
        const Viewport& viewport = _viewports[i];
        glViewport(viewport.X(), viewport.Y(), viewport.Width(), viewport.Height());
        glClear(viewport.clear_mask);
        if(viewport.clear_mask & GL_COLOR_BUFFER_BIT != 0)
        {
            const RGBA& color = viewport.clear_color;
            glClearColor(color.r, color.g, color.b, color.a);
        }
        if(viewport.clear_mask & GL_DEPTH_BUFFER_BIT != 0)
        {
            glClearDepth(viewport.clear_depth);
        }
        if(viewport.clear_mask & GL_STENCIL_BUFFER_BIT != 0)
        {
            glClearStencil(viewport.clear_stencil);
        }
    }

    /* Swap front and back buffers */
    glfwSwapBuffers(_window);
}

void GLFWMainWindow::HandleEvents()
{
    /* Poll for and process events */
    glfwPollEvents();
}

} // namespace TwinkleGraphics
