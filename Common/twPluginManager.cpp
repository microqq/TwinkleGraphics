

#include "twPluginManager.h"

namespace TwinkleGraphics
{

PluginManager::PluginManager(/* args */)
{
}

PluginManager::~PluginManager()
{
    for(auto p : _plugins)
    {

    }
}

void PluginManager::FindPlugins(std::string &dir)
{
}

Plugin *PluginManager::GetPlugin(PluginPath &path)
{
    return nullptr;
}

void PluginManager::InstallPlugin(Plugin *plugin)
{

}

void PluginManager::unInstallPlugin(Plugin *plugin)
{
}

} // namespace TwinkleGraphics
