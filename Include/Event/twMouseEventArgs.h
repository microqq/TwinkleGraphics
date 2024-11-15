#ifndef TW_MOUSEEVENTARGS_H
#define TW_MOUSEEVENTARGS_H

#include "twInput.h"
#include "twInputEventArgs.h"

namespace TwinkleGraphics {
class InputManager;

class MouseEventArgs : public InputEventArgs {
public:
  using Ptr = std::shared_ptr<MouseEventArgs> ;

  static EventId ID;

  explicit MouseEventArgs() : InputEventArgs() {}
  virtual ~MouseEventArgs() {}

  virtual EventId GetEventId() override { return MouseEventArgs::ID; }

  vec2 Position() { return _position; }
  MouseButton Button() { return _button; }
  MouseState State() { return _state; }

private:
  vec2 _position;
  MouseButton _button;
  MouseState _state;

  friend class InputManager;
};

} // namespace TwinkleGraphics

#endif