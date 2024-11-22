

#include "twDynLib.h"

#include "twConsoleLog.h"
#include "twPluginManager.h"


namespace TwinkleGraphics {

PluginManager::PluginManager(/* args */) {}

PluginManager::~PluginManager() { _plugins.clear(); }

Plugin *PluginManager::GetPlugin(PluginName &name) {
  MapPlugins::iterator it = _plugins.find(name);
  if (it != _plugins.end()) {
    return it->second;
  }

  return nullptr;
}

// 加载指定路径的插件并返回插件的指针
Plugin *PluginManager::LoadPlugin(std::string &path) {
  // 获取动态链接库管理器实例
  DynLibManager &dynlibMgr = DynLibManagerInst::Instance();
  // 从动态链接库管理器中加载指定路径的动态链接库
  DynLib *lib = dynlibMgr.Load(path);

  // 如果成功加载了动态链接库
  if (lib != nullptr) {
    // 定义安装插件的符号名称
    std::string installSymbol = "InstallPlugin";
    // 从动态链接库中获取安装插件的函数指针
    INSTALL_PLUGIN_FUNC installFunc =
        (INSTALL_PLUGIN_FUNC)lib->GetSymbol(installSymbol);

    // 如果成功获取了安装插件的函数指针
    if (installFunc != nullptr) {
      // 调用安装插件函数并返回插件的指针
      return installFunc(this);
    }
  }

  // 如果加载失败或获取符号失败，则返回空指针
  return nullptr;
}

void PluginManager::UnloadPlugin(std::string &path) {
  DynLibManager &dynlibMgr = DynLibManagerInst::Instance();
  DynLib *lib = dynlibMgr.GetDynLib(path);
  if (lib != nullptr) {
    std::string uninstallSymbol = "UnInstallPlugin";
    UNINSTALL_PLUGIN_FUNC uninstallFunc =
        (UNINSTALL_PLUGIN_FUNC)lib->GetSymbol(uninstallSymbol);

    if (uninstallFunc != nullptr) {
      uninstallFunc(this);
    }

    dynlibMgr.Unload(path);
  }
}

void PluginManager::InstallPlugin(Plugin *plugin) {
  if (plugin == nullptr)
    return;

  MapPlugins::iterator it = _plugins.find(plugin->GetName());
  if (it != _plugins.end()) {
    return;
  }

  plugin->Install();
  Console::LogInfo(plugin->GetName(), "\n");
  _plugins.insert(MapPlugins::value_type(plugin->GetName(), plugin));
}

void PluginManager::UnInstallPlugin(Plugin *plugin) {
  if (plugin == nullptr)
    return;

  MapPlugins::iterator it = _plugins.find(plugin->GetName());
  if (it == _plugins.end()) {
    return;
  }

  _plugins.erase(plugin->GetName());

  plugin->UnInstall();
}

} // namespace TwinkleGraphics
