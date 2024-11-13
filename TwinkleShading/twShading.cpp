
#include <exception>
#include <iostream>
#include <map>

#include "imgui.h"

#include "twCommon.h"
#include "twMainWindow.h"
#include "twPluginManager.h"
#include "twConsoleLog.h"
#include "twGLViewPlugin.h"

using namespace TwinkleGraphics;
using MapPluginsString = std::map<std::string, std::string>;

PluginManager &PluginMgr_ = PluginManagerInst::Instance();

static MapPluginsString PluginPaths_;
static std::string CurrentPlugin_ = std::string();
// create opengl window
static GLFWMainWindow MainWindow_(1024, 768);

void LoadPluginsGUI();
void UnLoadCurrentPlugin();
void MouseButtonInput(GLFWwindow *window, int32 button, int32 action,
                      int32 mods);
void CursorPosCallback(GLFWwindow *window, float64 xpos, float64 ypos);
void CursorEnterPosCallback(GLFWwindow *window, int32 entered);
void ScrollCallback(GLFWwindow *window, float64 dx, float64 dy);
void WindowSizeCallback(GLFWwindow *window, int, int);
void KeyCallback(GLFWwindow *window, int, int, int, int);

int main(int, char **) {

#if defined(__linux__) || defined(__APPLE__)
  PluginPaths_.insert(MapPluginsString::value_type(
      "1.FirstTriangle",
      "Output/libs/twShading/Debug/libtwFirstTriangle.dylib"));
  PluginPaths_.insert(MapPluginsString::value_type(
      "2.BasicGeometry",
      "Output/libs/twShading/Debug/libtwBasicGeometry.dylib"));
  PluginPaths_.insert(MapPluginsString::value_type(
      "3.TextureExplore",
      "Output/libs/twShading/Debug/libtwTextureExplore.dylib"));
  PluginPaths_.insert(MapPluginsString::value_type(
      "4.AntiAliasing", "Output/libs/twShading/Debug/libtwAntiAliasing.dylib"));
#elif defined _WIN32
  PluginPaths_.insert(MapPluginsString::value_type(
      "1.FirstTriangle", "Output/libtwFirstTriangle.dll"));
  PluginPaths_.insert(MapPluginsString::value_type(
      "2.BasicGeometry", "Output/libtwBasicGeometry.dll"));
  PluginPaths_.insert(MapPluginsString::value_type(
      "3.TextureExplore", "Output/libtwTextureExplore.dll"));
  PluginPaths_.insert(MapPluginsString::value_type(
      "4.AntiAliasing", "Output/libtwAntiAliasing.dll"));
#endif

  MainWindow_.InitWindow();

  IMGUI_FUNC loadPluginGuiFunc = (IMGUI_FUNC)LoadPluginsGUI;
  MainWindow_.AddGUIFunc(loadPluginGuiFunc);
  MainWindow_.SetMouseButtonInputCallback(MouseButtonInput);
  MainWindow_.SetCursorPosCallback(CursorPosCallback);
  MainWindow_.SetCursorPosEnterCallback(CursorEnterPosCallback);
  MainWindow_.SetScrollCallback(ScrollCallback);
  MainWindow_.SetWindowSizeCallback(WindowSizeCallback);
  MainWindow_.SetKeyCallback(KeyCallback);

  MainWindow_.InitOther();
  /**
   *
   * Debug Mode With GDB:
   *  Call ReadShaderAsync in plugin Antialiasing DLL, call future.get() which
   *  will throw exception with unknown/just-in-time compiled code.
   *
   *  So, fix this problem with solution that makes the operations of
   * _futures.push & future.get() in Common Update.
   */
  // ShaderManager& shaderMgr = ShaderMgrInstance();
  // ShaderOption* option = new ShaderOption(
  //     ShaderOption::OptionData{std::string("Assets/Shaders/line.vert"),
  //     ShaderType::VERTEX_SHADER});
  // shaderMgr.ReadShaderAsync("Assets/Shaders/line.vert", option);

  // main loop
  MainWindow_.Run();

  UnLoadCurrentPlugin();

  return 0;
}

void LoadPluginsGUI(void) {
  int32 pluginNum = PluginPaths_.size();
  MapPluginsString::iterator it = PluginPaths_.begin();

  ImGui::Begin(u8"Shading(OpenGL)", NULL);
  for (int32 i = 0; i < pluginNum; i++) {
    if (ImGui::Button((it->first).c_str())) {
      if (CurrentPlugin_.compare(it->first.c_str()) != 0) {
        if (!(CurrentPlugin_.empty())) {
          // unload last plugin
          UnLoadCurrentPlugin();
        }

        // load plugin
        GLViewPlugin *plugin =
            dynamic_cast<GLViewPlugin *>(PluginMgr_.LoadPlugin(it->second));
        if (plugin != nullptr && plugin->GetViewsCount() > 0) {
          MainWindow_.AddViews(plugin->GetViews(), plugin->GetViewsCount());
          plugin->UpdateViews();
        }
      }
      CurrentPlugin_ = it->first;
    }

    it++;
  }
  ImGui::End();
}

void UnLoadCurrentPlugin() {
  std::string lastPluginName = CurrentPlugin_;
  GLViewPlugin *last_plugin =
      dynamic_cast<GLViewPlugin *>(PluginMgr_.GetPlugin(lastPluginName));
  if (last_plugin != nullptr) {
    TwinkleGraphics::View **views = last_plugin->GetViews();
    MainWindow_.RemoveViews(views, last_plugin->GetViewsCount());

    MapPluginsString::iterator lastPluginPath =
        PluginPaths_.find(lastPluginName);
    PluginMgr_.UnloadPlugin(lastPluginPath->second);
  }
}

void MouseButtonInput(GLFWwindow *window, int32 button, int32 action,
                      int32 mods) {
  MainWindow_.MouseButtonInput(button, action, mods);
}

void CursorPosCallback(GLFWwindow *window, float64 xpos, float64 ypos) {
  MainWindow_.CursorPosCallback(xpos, ypos);
}

void CursorEnterPosCallback(GLFWwindow *window, int32 entered) {
  MainWindow_.CursorEnterPosCallback(entered);
}

void ScrollCallback(GLFWwindow *window, float64 dx, float64 dy) {
  MainWindow_.ScrollCallback(dx, dy);
}

void WindowSizeCallback(GLFWwindow *window, int w, int h) {
  MainWindow_.WindowSizeCallback(w, h);
}

void KeyCallback(GLFWwindow *window, int key, int scannode, int action,
                 int mods) {
  MainWindow_.KeyCallBack(key, scannode, action, mods);
}