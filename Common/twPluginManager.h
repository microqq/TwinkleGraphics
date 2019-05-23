
#ifndef TW_PLUGINMANAGER_H
#define TW_PLUGINMANAGER_H

#include <map>

#include "twCommon.h"
#include "twPlugin.h"

namespace TwinkleGraphics
{
typedef std::string PluginPath;
typedef std::string PluginName;

class PluginManager;
typedef Singleton<PluginManager> PluginManagerInst;

class PluginManager
{
public:
    PluginManager(/* args */);
    ~PluginManager();

    void FindPlugins(std::string& dir);
    Plugin* GetPlugin(PluginPath& path);
    void InstallPlugin(Plugin* plugin);
    void unInstallPlugin(Plugin* plugin);

private:
    std::map<PluginPath, Plugin*> _plugins;
};

}

#endif