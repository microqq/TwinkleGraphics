

#include "twCommon.h"
#include "twPluginManager.h"

#include  "twFirstTriangle.h"

namespace TwinkleGraphics
{
    static FirstTriangle* plugin = nullptr;
    extern "C" __TWExport Plugin* InstallPlugin(PluginManager*);
    extern "C" __TWExport void UnInstallPlugin(PluginManager*);

    extern "C" __TWExport Plugin *InstallPlugin(PluginManager *pluginMgr)
    {
        std::string name = "1.FirstTriangle";
        plugin = new FirstTriangle(name);

        pluginMgr->InstallPlugin(plugin);

        return plugin;
    }

    extern "C" __TWExport void UnInstallPlugin(PluginManager *pluginMgr)
    {
        pluginMgr->UnInstallPlugin(plugin);
        SAFE_DEL(plugin);
    }
} // namespace TwinkleGraphics
