#include "twApplication.h"
#include "twEditor.h"

using namespace TwinkleGraphics;

int main(int, char **)
{   
    Application& app = ApplicationInstance();
    EditorWindow editor(1024, 768);

    app.Run();
    return 0;
}