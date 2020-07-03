

#include "twCommon.h"
#include "twPluginManager.h"

#include  "twAntiAliasing.h"

namespace TwinkleGraphics
{
    static AntiAliasing* plugin = nullptr;
    extern "C" __TWExport Plugin *InstallPlugin(PluginManager *);
    extern "C" __TWExport void UnInstallPlugin(PluginManager*);

    extern "C" __TWExport Plugin* InstallPlugin(PluginManager* pluginMgr)
    {
        std::string name = "4.AntiAliasing";
        plugin = new AntiAliasing(name);

        pluginMgr->InstallPlugin(plugin);

        return plugin;
    }

    extern "C" __TWExport void UnInstallPlugin(PluginManager* pluginMgr)
    {
        pluginMgr->UnInstallPlugin(plugin);
        SAFE_DEL(plugin);
    }
} // namespace TwinkleGraphics
