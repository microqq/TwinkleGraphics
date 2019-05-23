
#ifndef TW_PLUGIN_H
#define TW_PLUGIN_H

#include <string>

namespace TwinkleGraphics
{

class Plugin
{
public:
    Plugin(std::string& name);
    ~Plugin();

    virtual void Install() = 0;
    virtual void UnInstall() = 0;

protected:
    std::string _name;
};
} // namespace TwinkleGraphics

#endif