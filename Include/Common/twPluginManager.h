
#ifndef TW_PLUGINMANAGER_H
#define TW_PLUGINMANAGER_H

#include <map>

#include "twCommon.h"
#include "twPlugin.h"

namespace TwinkleGraphics {
typedef std::string PluginName;

class PluginManager;
typedef Singleton<PluginManager> PluginManagerInst;

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
  typedef std::map<PluginName, Plugin *> MapPlugins;

  std::map<PluginName, Plugin *> _plugins;
};

} // namespace TwinkleGraphics

#endif