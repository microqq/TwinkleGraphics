
#include <iostream>
#include <vector>
#include <exception>

#include "twCommon.h"
#include "twPluginManager.h"
#include "twMainWindow.h"

using namespace TwinkleGraphics;

static std::vector<std::string> PluginPaths;

int main(int, char **)
{
    PluginManagerInst pluginMgr;

#ifdef __linux__
    PluginPaths.push_back("Output/libs/twShading/libtwFirstTriangle.so");
#elif defined _WIN32 || _WIN64
    PluginPaths.push_back("Output/libs/twShading/libtwFirstTriangle.dll");
#endif
    //create opengl window
    GLFWMainWindow mainWindow;

    try
    {
        GLPlugin *plugin = dynamic_cast<GLPlugin *>(pluginMgr->LoadPlugin(PluginPaths[0]));
        if (plugin != nullptr && plugin->GetViewsCount() > 0)
        {
            mainWindow.AddViews(plugin->GetViews(), plugin->GetViewsCount());
        }
    }
    catch(std::exception e)
    {
        throw e;
    }

    //main loop
    mainWindow.Run();

    //unload plugins
    pluginMgr->UnloadPlugin(PluginPaths[0]);

    return 0;
}
