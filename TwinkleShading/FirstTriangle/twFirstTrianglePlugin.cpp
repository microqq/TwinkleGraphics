

#include "twCommon.h"
#include "twPluginManager.h"

#include "twFirstTriangle.h"

namespace TwinkleGraphics {
static FirstTriangle *plugin = nullptr;

#ifdef __cplusplus
extern "C" {
#endif
__TWPLUGINAPI Plugin *InstallPlugin(PluginManager *);
__TWPLUGINAPI void UnInstallPlugin(PluginManager *);

Plugin *InstallPlugin(PluginManager *pluginMgr) {
  std::string name = "1.FirstTriangle";
  plugin = new FirstTriangle(name);

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
