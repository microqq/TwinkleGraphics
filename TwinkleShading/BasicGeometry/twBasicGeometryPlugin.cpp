

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
        __TWPLUGINExport Plugin *InstallPlugin(PluginManager *);
        __TWPLUGINExport void UnInstallPlugin(PluginManager *);

        Plugin *InstallPlugin(PluginManager *pluginMgr)
        {
            std::string name = "2.BasicGeometry";
            plugin = new BasicGeometry(name);

            pluginMgr->InstallPlugin(plugin);

            return plugin;
        }

        void UnInstallPlugin(PluginManager *pluginMgr)
        {
            pluginMgr->UnInstallPlugin(plugin);
            SAFE_DEL(plugin);
        }
#ifdef __cplusplus
    }
#endif

} // namespace TwinkleGraphics
