#include "twResourceManager.h"

namespace TwinkleGraphics
{
    ResourceManager& ResourceMgrInstance() { return Singleton<ResourceManager>::Instance(); }
    
} // namespace TwinkleGraphics
