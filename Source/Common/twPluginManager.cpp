

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

Plugin *PluginManager::LoadPlugin(std::string &path) {
  DynLibManager &dynlibMgr = DynLibManagerInst::Instance();
  DynLib *lib = dynlibMgr.Load(path);
  if (lib != nullptr) {
    std::string installSymbol = "InstallPlugin";
    INSTALL_PLUGIN_FUNC installFunc =
        (INSTALL_PLUGIN_FUNC)lib->GetSymbol(installSymbol);

    if (installFunc != nullptr) {
      return installFunc(this);
    }
  }

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
