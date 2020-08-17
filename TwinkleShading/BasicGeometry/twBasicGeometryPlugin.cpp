

#include "twCommon.h"
#include "twPluginManager.h"

#include "twBasicGeometry.h"

namespace TwinkleGraphics
{
    static BasicGeometry *plugin = nullptr;
#ifdef __cplusplus
    extern "C"
    {
#endif
        __TWExport Plugin *InstallPlugin(PluginManager *);
        __TWExport void UnInstallPlugin(PluginManager *);

        __TWExport Plugin *InstallPlugin(PluginManager *pluginMgr)
        {
            std::string name = "2.BasicGeometry";
            plugin = new BasicGeometry(name);

            pluginMgr->InstallPlugin(plugin);

            return plugin;
        }

        __TWExport void UnInstallPlugin(PluginManager *pluginMgr)
        {
            pluginMgr->UnInstallPlugin(plugin);
            SAFE_DEL(plugin);
        }
#ifdef __cplusplus
    }
#endif

} // namespace TwinkleGraphics
