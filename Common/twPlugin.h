
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

    virtual void Install() {}
    virtual void UnInstall() {}
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

    virtual void Install() override {}
    virtual void UnInstall() override {}

    int GetViewsCount() { return _views_count;  }
    View** GetViews() { return _views; }
    void AddView(View* view) { _views[_views_count++] = view; }

protected:
    View* _views[MAX_VIEWPORT_COUNT];
    int _views_count;
};
} // namespace TwinkleGraphics

#endif