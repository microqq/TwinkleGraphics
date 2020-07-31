
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

    std::string& GetName() { return _name; }

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
        , _viewsCount(0)
    {}
    virtual ~GLPlugin() {}

    virtual void UpdateViews() {}
    int GetViewsCount() { return _viewsCount; }
    View** GetViews() { return _views; }

protected:
    glm::ivec2 _windowSize;

    View* _views[MAX_VIEWPORT_COUNT];
    int _viewsCount;
};
} // namespace TwinkleGraphics

#endif