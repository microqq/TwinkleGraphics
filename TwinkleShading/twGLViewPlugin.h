#ifndef TW_GLVIEWPLUGIN_H
#define TW_GLVIEWPLUGIN_H

#include "twPlugin.h"
#include "twView.h"

namespace TwinkleGraphics {
class GLViewPlugin : public Plugin {
public:
  GLViewPlugin(std::string &name) : Plugin(name), _views(), _viewsCount(0) {}
  virtual ~GLViewPlugin() {}

  virtual void UpdateViews() {}
  int GetViewsCount() { return _viewsCount; }
  View **GetViews() { return _views; }

protected:
  glm::ivec2 _windowSize;

  View *_views[MAX_VIEWPORT_COUNT];
  int _viewsCount;
};
}

#endif
