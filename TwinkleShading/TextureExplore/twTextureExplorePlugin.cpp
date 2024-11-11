

#include "twCommon.h"
#include "twPluginManager.h"

#include "twTextureExplore.h"

namespace TwinkleGraphics {
static TextureExplore *plugin = nullptr;

#ifdef __cplusplus
extern "C" {
#endif
__TWPLUGINAPI Plugin *InstallPlugin(PluginManager *);
__TWPLUGINAPI void UnInstallPlugin(PluginManager *);

Plugin *InstallPlugin(PluginManager *pluginMgr) {
  std::string name = "3.TextureExplore";
  plugin = new TextureExplore(name);

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
