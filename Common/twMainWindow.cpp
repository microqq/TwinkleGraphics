
#include "twMainWindow.h"

namespace TwinkleGraphics
{
MainWindow::MainWindow(int width, int height)
    : _views()
    , _width(width)
    , _height(height)
    , _view_count(0)
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
        || _view_count == MAX_VIEWPORT_COUNT
    )
        return;

    _views[_view_count++] = view;
}

GLFWMainWindow::GLFWMainWindow(int32 width, int32 height)
    : MainWindow(width, height)
{
    Initialise();
}

GLFWMainWindow::~GLFWMainWindow()
{
    Terminate();
}

void GLFWMainWindow::Run()
{
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(_window))
    {
        HandleEvents();

        if(_view_count > 0)
        {
            for(int i = 0; i < _view_count; i++)
            {
                _views[i]->Run();
            }
        }
        else
        {
            glViewport(0, 0, _width, _height);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glClearColor(0.f, 0.0f, 0.0f, 1.0f);
        }

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
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

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
    _window = nullptr;
}

void GLFWMainWindow::HandleEvents()
{
    /* Poll for and process events */
    glfwPollEvents();
}

} // namespace TwinkleGraphics