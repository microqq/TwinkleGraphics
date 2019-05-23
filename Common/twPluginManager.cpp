

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

Plugin *PluginManager::GetPlugin(PluginName &path)
{
    return nullptr;
}

void PluginManager::InstallPlugin(Plugin *plugin)
{

}

void PluginManager::UnInstallPlugin(Plugin *plugin)
{
}

} // namespace TwinkleGraphics
