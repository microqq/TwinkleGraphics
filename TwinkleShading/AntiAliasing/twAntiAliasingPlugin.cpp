

#include "twCommon.h"
#include "twPluginManager.h"

#include "twAntiAliasing.h"

namespace TwinkleGraphics {
static AntiAliasing *plugin = nullptr;

#ifdef __cplusplus
extern "C" {
#endif
__TWPLUGINAPI Plugin *InstallPlugin(PluginManager *);
__TWPLUGINAPI void UnInstallPlugin(PluginManager *);

Plugin *InstallPlugin(PluginManager *pluginMgr) {
  std::string name = "4.AntiAliasing";
  plugin = new AntiAliasing(name);

  pluginMgr->InstallPlugin(plugin);

  return plugin;
}

void UnInstallPlugin(PluginManager *pluginMgr) {
  pluginMgr->UnInstallPlugin(plugin);
  SAFE_DEL(plugin);
}
#ifdef __cplusplus
}
#endif

} // namespace TwinkleGraphics
