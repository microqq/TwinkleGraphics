
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>

//// brew install glm
///https://blog.csdn.net/weixin_45669613/article/details/114340388
///https://www.cnblogs.com/orzs/p/14182070.html
// 遇到 undefined method ‘cellar‘ for
// cd /usr/local/Homebrew/Library/Taps/homebrew/
// rm -rf homebrew-core
// git clone https://github.com/Homebrew/homebrew-core.git

///==> Running `brew cleanup glm`...
///Disable this behaviour by setting HOMEBREW_NO_INSTALL_CLEANUP
//

void Render(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    {
        glColor3f(1.0f,0.0f,0.0f);
        glVertex2f(0.0f, .5f);
        glColor3f(0.0f,1.0f,0.0f);
        glVertex2f(-.5f,-.5f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(.5f, -.5f);
    }
    glEnd();
}

int main(int argc, const char * argv[]) {
    GLFWwindow* win;
    if(!glfwInit()){
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    win = glfwCreateWindow(640, 480, "OpenGL Base Project", NULL, NULL);
    if(!win)
    {
        glfwTerminate();
        return -1;
    }
    if(!glewInit())
    {
        return -1;
    }
    glfwMakeContextCurrent(win);

    char *version = (char *)glGetString(GL_VERSION);
    std::cout << "OpenGL Version:=" << version << std::endl;

    while(!glfwWindowShouldClose(win)){
        Render();
        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    glfwTerminate();
    return 1;
}
