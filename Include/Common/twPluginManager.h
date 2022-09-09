
#ifndef TW_PLUGINMANAGER_H
#define TW_PLUGINMANAGER_H

#include <map>

#include "twCommon.h"
#include "twPlugin.h"

namespace TwinkleGraphics {
using PluginName = std::string;

class PluginManager;
using PluginManagerInst = Singleton<PluginManager>;
using MapPlugins = std::map<PluginName, Plugin *>;

class __TWCOMExport PluginManager {
public:
  PluginManager(/* args */);
  ~PluginManager();

  Plugin *LoadPlugin(std::string &path);
  void UnloadPlugin(std::string &path);

  Plugin *GetPlugin(PluginName &name);
  void InstallPlugin(Plugin *plugin);
  void UnInstallPlugin(Plugin *plugin);

private:
  MapPlugins _plugins;
};

} // namespace TwinkleGraphics

#endif