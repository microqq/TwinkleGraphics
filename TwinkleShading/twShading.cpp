#include <iostream>

#include "twCommon.h"
#include "twMainWindow.h"

using namespace TwinkleGraphics;

int main(int, char **)
{
    std::cout << "Twinkle Shading...\n";

    GLFWMainWindow mainWindow;
    mainWindow.Run();

    return 0;
}
