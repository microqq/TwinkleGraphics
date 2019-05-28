

#include "twResource.hpp"

namespace TwinkleGraphics
{
ResourceReader::ResourceReader(const char *type)
    : _type(type)
{
    ResourceManagerInst resMgr;
    resMgr->RegisterReader(type, this);
}
ResourceReader::~ResourceReader()
{
    ResourceManagerInst resMgr;
    resMgr->UnRegisterReader(_type.c_str());
}

} // namespace TwinkleGraphics
