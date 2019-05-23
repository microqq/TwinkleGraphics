

#include  "twFirstTriangle.h"

namespace TwinkleGraphics
{
    static FirstTriangle* plugin;
    extern "C" void InstallPlugin();
    extern "C" void UnInstallPlugin();

    extern "C" void InstallPlugin()
    {}
    
    extern "C" void UnInstallPlugin()
    {}

} // namespace TwinkleGraphics
