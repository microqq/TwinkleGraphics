
#include <iostream>
#include <vector>
#include <exception>

#include "imgui.h"

#include "twCommon.h"
#include "twPluginManager.h"
#include "twMainWindow.h"

using namespace TwinkleGraphics;

static std::vector<std::string> PluginPaths;

void LoadPluginsGUI();

int main(int, char **)
{
    PluginManagerInst pluginMgr;

#ifdef __linux__
    PluginPaths.push_back("Output/libs/twShading/libtwFirstTriangle.so");
    PluginPaths.push_back("Output/libs/twShading/libtwBasicGeometry.so");
#elif defined _WIN32 || _WIN64
    PluginPaths.push_back("Output/libs/twShading/libtwFirstTriangle.dll");
    PluginPaths.push_back("Output/libs/twShading/libtwBasicGeometry.dll");
#endif
    //create opengl window
    GLFWMainWindow mainWindow;

    IMGUI_FUNC load_plugin_gui_fun = (IMGUI_FUNC)LoadPluginsGUI;
    mainWindow.AddGUIFunc(load_plugin_gui_fun);

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

void LoadPluginsGUI(void)
{
    ImGui::Begin("Shading...");
    ImGui::End();
}
