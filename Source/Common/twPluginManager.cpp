

#include "twDynLib.h"

#include "twPluginManager.h"
#include "twConsoleLog.h"

namespace TwinkleGraphics
{

PluginManager::PluginManager(/* args */)
{
}

PluginManager::~PluginManager()
{
    _plugins.clear();
}

Plugin *PluginManager::GetPlugin(PluginName &name)
{
    MapPlugins::iterator it = _plugins.find(name);
    if(it != _plugins.end())
    {
        return it->second;
    }

    return nullptr;
}

Plugin* PluginManager::LoadPlugin(std::string &path)
{
    DynLibManager& dynlibMgr = DynLibManagerInst::Instance();
    DynLib* lib = dynlibMgr.Load(path);
    if(lib != nullptr)
    {
        std::string install_symbol = "InstallPlugin";
        INSTALL_PLUGIN_FUNC install_func = 
            (INSTALL_PLUGIN_FUNC)lib->GetSymbol(install_symbol);

        if(install_func != nullptr)
        {
            return install_func(this);
        }
    }

    return nullptr;
}

void PluginManager::UnloadPlugin(std::string &path)
{
    DynLibManager& dynlibMgr = DynLibManagerInst::Instance();
    DynLib* lib = dynlibMgr.GetDynLib(path);
    if(lib != nullptr)
    {
        std::string uninstall_symbol = "UnInstallPlugin";
        UNINSTALL_PLUGIN_FUNC uninstall_func = 
            (UNINSTALL_PLUGIN_FUNC)lib->GetSymbol(uninstall_symbol);

        if(uninstall_func != nullptr)
        {
            uninstall_func(this);
        }        

        dynlibMgr.Unload(path);
    }
}

void PluginManager::InstallPlugin(Plugin *plugin)
{
    if(plugin == nullptr)
        return;

    MapPlugins::iterator it = _plugins.find(plugin->GetName());
    if(it != _plugins.end())
    {
        return;
    }

    plugin->Install();
    Console::LogInfo(plugin->GetName(), "\n");
    _plugins.insert(MapPlugins::value_type(plugin->GetName(), plugin));
}

void PluginManager::UnInstallPlugin(Plugin *plugin)
{
    if(plugin == nullptr)
        return;

    MapPlugins::iterator it = _plugins.find(plugin->GetName());
    if(it == _plugins.end())
    {
        return;
    }

    _plugins.erase(plugin->GetName());

    plugin->UnInstall();
}

} // namespace TwinkleGraphics
