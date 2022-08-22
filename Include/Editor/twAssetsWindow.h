#ifndef TW_ASSETS_WINDOW
#define TW_ASSETS_WINDOW

#include "twWindow.h"

namespace TwinkleGraphics {
class __TWCOMExport AssetsWindow : public Window {
public:
  explicit AssetsWindow(const std::string &name, uint32 width, uint32 height,
                        Widget *parent = nullptr);
  virtual ~AssetsWindow();

  virtual void Update(float deltaTime = 0.0f) override;
  virtual void OnGuiBegin() override;
  virtual void OnGui() override;
  virtual void OnGuiEnd() override;

private:
  virtual void SetFocusedInternal() override;
  virtual void SetHoveredInternal() override;
};
} // namespace TwinkleGraphics

#endif
