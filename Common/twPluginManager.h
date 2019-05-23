
#ifndef TW_PLUGINMANAGER_H
#define TW_PLUGINMANAGER_H

#include <map>

#include "twCommon.h"
#include "twPlugin.h"

namespace TwinkleGraphics
{
typedef std::string PluginName;

class PluginManager;
typedef Singleton<PluginManager> PluginManagerInst;

class PluginManager
{
public:
    PluginManager(/* args */);
    ~PluginManager();

    void FindPlugins(std::string& dir);
    Plugin* GetPlugin(PluginName& path);
    void InstallPlugin(Plugin* plugin);
    void UnInstallPlugin(Plugin* plugin);

private:
    std::map<PluginName, Plugin*> _plugins;
};

}

#endif