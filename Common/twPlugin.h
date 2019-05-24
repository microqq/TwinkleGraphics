
#ifndef TW_PLUGIN_H
#define TW_PLUGIN_H

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
    ~Plugin() {}

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

protected:
    void InstallDefaultView()
    {
        Viewport viewport(Rect(0, 0, 1024, 768), 17664U, RGBA(1.0f, 0.f, 0.f, 1.f));
        _views[0] = new View(viewport);

        _views_count = 1;
    }

protected:
    View* _views[MAX_VIEWPORT_COUNT];
    int _views_count;
};
} // namespace TwinkleGraphics

#endif