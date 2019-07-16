
#include <iostream>
#include <map>
#include <exception>

#include "imgui.h"

#include "twCommon.h"
#include "twPluginManager.h"
#include "twMainWindow.h"

using namespace TwinkleGraphics;
using MapPlugins = std::map<std::string, std::string>;

PluginManagerInst pluginMgr;

static MapPlugins PluginPaths;
static std::string CurrentPlugin = std::string();
//create opengl window
static GLFWMainWindow mainWindow;

void LoadPluginsGUI();
void UnLoadCurrentPlugin();
void MouseButtonInput(GLFWwindow* window, int32 button, int32 action, int32 mods);
void CursorPosCallback(GLFWwindow* window, float64 xpos, float64 ypos);
void CursorEnterPosCallback(GLFWwindow* window, int32 entered);
void ScrollCallback(GLFWwindow* window, float64 dx, float64 dy);

int main(int, char **)
{

#ifdef __linux__
    PluginPaths.insert(MapPlugins::value_type("1.FirstTriangle", "Output/libs/twShading/libtwFirstTriangle.so"));
    PluginPaths.insert(MapPlugins::value_type("2.BasicGeometry", "Output/libs/twShading/libtwBasicGeometry.so"));
#elif defined _WIN32 || _WIN64
    PluginPaths.insert(MapPlugins::value_type("1.FirstTriangle", "Output/libs/twShading/libtwFirstTriangle.dll"));
    PluginPaths.insert(MapPlugins::value_type("2.BasicGeometry", "Output/libs/twShading/libtwBasicGeometry.dll"));
#endif

    IMGUI_FUNC load_plugin_gui_fun = (IMGUI_FUNC)LoadPluginsGUI;
    mainWindow.AddGUIFunc(load_plugin_gui_fun);
    mainWindow.SetMouseButtonInputCallback(MouseButtonInput);
    mainWindow.SetCursorPosCallback(CursorPosCallback);
    mainWindow.SetCursorPosEnterCallback(CursorEnterPosCallback);
    mainWindow.SetScrollCallback(ScrollCallback);

    //main loop
    mainWindow.Run();

    UnLoadCurrentPlugin();

    return 0;
}

void LoadPluginsGUI(void)
{
    int32 plugin_num = PluginPaths.size();
    MapPlugins::iterator it = PluginPaths.begin();
    
    ImGui::Begin(u8"Shading(OpenGL)");
    for(int32 i = 0; i < plugin_num; i++)
    {
        if(ImGui::Button((it->first).c_str()))
        {
            if(CurrentPlugin.compare(it->first.c_str()) != 0)
            {
                if(!(CurrentPlugin.empty()))
                {
                    //unload last plugin
                    UnLoadCurrentPlugin();
                }

                //load plugin
                GLPlugin *plugin = dynamic_cast<GLPlugin *>(pluginMgr->LoadPlugin(it->second));
                if (plugin != nullptr && plugin->GetViewsCount() > 0)
                {
                    mainWindow.AddViews(plugin->GetViews(), plugin->GetViewsCount());
                    mainWindow.Reset();
                }
            }
            CurrentPlugin = it->first;
        }


        it++;
    }
    ImGui::End();
}

void UnLoadCurrentPlugin()
{
    std::string last_plugin_name = CurrentPlugin;
    GLPlugin *last_plugin = dynamic_cast<GLPlugin *>(pluginMgr->GetPlugin(last_plugin_name));
    if(last_plugin != nullptr)
    {
        TwinkleGraphics::View **views = last_plugin->GetViews();
        mainWindow.RemoveViews(views, last_plugin->GetViewsCount());

        MapPlugins::iterator last_plugin_path = PluginPaths.find(last_plugin_name);
        pluginMgr->UnloadPlugin(last_plugin_path->second);
    }
}


void MouseButtonInput(GLFWwindow* window, int32 button, int32 action, int32 mods)
{
    mainWindow.MouseButtonInput(button, action, mods);
}

void CursorPosCallback(GLFWwindow* window, float64 xpos, float64 ypos)
{
    mainWindow.CursorPosCallback(xpos, ypos);
}

void CursorEnterPosCallback(GLFWwindow* window, int32 entered)
{
    mainWindow.CursorEnterPosCallback(entered);
}

void ScrollCallback(GLFWwindow* window, float64 dx, float64 dy)
{
    mainWindow.ScrollCallback(dx, dy);
}