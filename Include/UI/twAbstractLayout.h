#ifndef TW_ABSTRACTLAYOUT_H
#define TW_ABSTRACTLAYOUT_H

namespace TwinkleGraphics {
class Widget;

enum Alignment {
  ALIGN_LEFT = 0,
  ALIGN_RIGHT = 1,
  ALIGN_HCENTER = 2,
  ALIGN_VCENTER = 3,
  ALIGN_TOP = 4,
  ALIGN_BOTTOM = 5,
  ALIGN_CENTER = 6,
};

enum LayoutType { GRID_LAYOUT = 0, HORIZONTAL_LAYOUT = 1, VERTICAL_LAYOUT = 2 };

class AbstractLayout {
public:
  explicit AbstractLayout(Widget *host) : _hostWidget(host) {}
  virtual ~AbstractLayout() { _hostWidget = nullptr; }

  virtual void Begin() = 0;
  virtual void End() = 0;
  virtual void AddWidget(Widget *widget, Alignment alignment = ALIGN_LEFT) = 0;

  void SetHostWidget(Widget *host) { _hostWidget = host; }
  Widget *GetHostWidget() { return _hostWidget; }

protected:
  Widget *_hostWidget{nullptr};
};
} // namespace TwinkleGraphics

#endif
