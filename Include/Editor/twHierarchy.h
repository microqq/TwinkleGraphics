#ifndef TW_HIERARCHY_H
#define TW_HIERARCHY_H

#include <string>
#include <vector>

#include "twWidget.h"

namespace TwinkleGraphics {
struct HierarchyItem {
  std::string name;
};

class __TWAPI Hierarchy : public Widget {
public:
  explicit Hierarchy(const std::string &name, Widget *parent = nullptr);
  virtual ~Hierarchy();

protected:
  virtual void OnGuiBegin() override;
  virtual void OnGui() override;
  virtual void OnGuiEnd() override;

private:
  std::vector<HierarchyItem> _items;
};

} // namespace TwinkleGraphics

#endif