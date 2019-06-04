

#include "twCommon.h"
#include "twPluginManager.h"

#include  "twBasicGeometry.h"

namespace TwinkleGraphics
{
    static BasicGeometry* plugin = nullptr;
    extern "C" __TWExport Plugin* InstallPlugin();
    extern "C" __TWExport void UnInstallPlugin();

    extern "C" __TWExport Plugin* InstallPlugin()
    {
        std::string name = "BasicGeometry";
        plugin = new BasicGeometry(name);

        PluginManagerInst pluginMgr;
        pluginMgr->InstallPlugin(plugin);

        return plugin;
    }

    extern "C" __TWExport void UnInstallPlugin()
    {
        PluginManagerInst pluginMgr;
        pluginMgr->UnInstallPlugin(plugin);
        SAFE_DEL(plugin);
    }
} // namespace TwinkleGraphics
