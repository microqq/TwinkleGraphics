#ifndef TW_INSPECTOR_H
#define TW_INSPECTOR_H

#include "twWidget.h"

namespace TwinkleGraphics {
class __TWAPI Inspector : public Widget {
public:
  explicit Inspector(const std::string &name, Widget *parent = nullptr);
  virtual ~Inspector();

protected:
  virtual void OnGuiBegin() override;
  virtual void OnGui() override;
  virtual void OnGuiEnd() override;

private:
};
} // namespace TwinkleGraphics

#endif