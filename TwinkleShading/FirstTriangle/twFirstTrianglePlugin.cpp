

#include  "twFirstTriangle.h"
#include "twPluginManager.h"

namespace TwinkleGraphics
{
    static FirstTriangle* plugin;
    extern "C" void InstallPlugin();
    extern "C" void UnInstallPlugin();

    extern "C" void InstallPlugin()
    {
        std::string name = "FirstTriangle";
        plugin = new FirstTriangle(name);

        PluginManagerInst pluginMgr;
        pluginMgr->InstallPlugin(plugin);
    }

    extern "C" void UnInstallPlugin()
    {
        PluginManagerInst pluginMgr;
        pluginMgr->UnInstallPlugin(plugin);
    }

} // namespace TwinkleGraphics
