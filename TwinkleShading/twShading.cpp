
#include <iostream>
#include <vector>

#include "twCommon.h"
#include "twPluginManager.h"
#include "twMainWindow.h"

using namespace TwinkleGraphics;

static std::vector<std::string> PluginPaths;

int main(int, char **)
{
    PluginManagerInst pluginMgr;

    PluginPaths.push_back("Output/libs/twShading/libtwFirstTriangle.so");

    //create opengl window
    GLFWMainWindow mainWindow;

    GLPlugin* plugin = dynamic_cast<GLPlugin*>(pluginMgr->LoadPlugin(PluginPaths[0]));
    if(plugin != nullptr && plugin->GetViewsCount() > 0)
    {
        mainWindow.AddViews(plugin->GetViews(), plugin->GetViewsCount());
    }

    //main loop
    mainWindow.Run();

    //unload plugins
    pluginMgr->UnloadPlugin(PluginPaths[0]);

    return 0;
}
