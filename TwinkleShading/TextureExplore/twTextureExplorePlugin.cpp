

#include "twCommon.h"
#include "twPluginManager.h"

#include  "twTextureExplore.h"

namespace TwinkleGraphics
{
    static TextureExplore* plugin = nullptr;
    extern "C" __TWExport Plugin *InstallPlugin(PluginManager *);
    extern "C" __TWExport void UnInstallPlugin(PluginManager*);

    extern "C" __TWExport Plugin* InstallPlugin(PluginManager* pluginMgr)
    {
        std::string name = "3.TextureExplore";
        plugin = new TextureExplore(name);

        pluginMgr->InstallPlugin(plugin);

        return plugin;
    }

    extern "C" __TWExport void UnInstallPlugin(PluginManager* pluginMgr)
    {
        pluginMgr->UnInstallPlugin(plugin);
        SAFE_DEL(plugin);
    }
} // namespace TwinkleGraphics
