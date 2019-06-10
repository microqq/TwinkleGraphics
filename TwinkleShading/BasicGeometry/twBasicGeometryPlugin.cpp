

#include "twCommon.h"
#include "twPluginManager.h"

#include  "twBasicGeometry.h"

namespace TwinkleGraphics
{
    static BasicGeometry* plugin = nullptr;
    extern "C" __TWExport Plugin *InstallPlugin(PluginManager *);
    extern "C" __TWExport void UnInstallPlugin(PluginManager*);

    extern "C" __TWExport Plugin* InstallPlugin(PluginManager* pluginMgr)
    {
        std::string name = "2.BasicGeometry";
        plugin = new BasicGeometry(name);

        pluginMgr->InstallPlugin(plugin);

        return plugin;
    }

    extern "C" __TWExport void UnInstallPlugin(PluginManager* pluginMgr)
    {
        pluginMgr->UnInstallPlugin(plugin);
        SAFE_DEL(plugin);
    }
} // namespace TwinkleGraphics
