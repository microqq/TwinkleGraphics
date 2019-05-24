

#include "twCommon.h"
#include "twPluginManager.h"

#include  "twFirstTriangle.h"

namespace TwinkleGraphics
{
    static FirstTriangle* plugin = nullptr;
    extern "C" __TWExport Plugin* InstallPlugin();
    extern "C" __TWExport void UnInstallPlugin();

    extern "C" __TWExport Plugin* InstallPlugin()
    {
        std::string name = "FirstTriangle";
        plugin = new FirstTriangle(name);

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
