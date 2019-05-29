
#ifndef TW_PLUGIN_H
#define TW_PLUGIN_H

#include <iostream>
#include <string>

#include "twView.h"

namespace TwinkleGraphics
{
class Plugin
{
public:
    Plugin(std::string& name)
        : _name(name)
    {}
    virtual ~Plugin() {}

    inline std::string& GetName() { return _name; }

    virtual void Install() 
    {
        std::cout << "-------------------------------" << "Plugin Install:" 
            << _name << "-------------------------------" << "\n";
    }
    virtual void UnInstall() 
    {
        std::cout << "------------------------------" << "Plugin UnInstall:"
            << _name << "------------------------------" << "\n";
    }
protected:
    std::string _name;
};

class GLPlugin : public Plugin
{
public:
    GLPlugin(std::string& name)
        : Plugin(name)
        , _views()
        , _views_count(0)
    {}
    virtual ~GLPlugin() {}

    int GetViewsCount() { return _views_count; }
    View** GetViews() { return _views; }

protected:
    View* _views[MAX_VIEWPORT_COUNT];
    int _views_count;
};
} // namespace TwinkleGraphics

#endif