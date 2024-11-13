#ifndef TW_CURSOREVENTARGS_H
#define TW_CURSOREVENTARGS_H

#include "twInputEventArgs.h"

namespace TwinkleGraphics {
class InputManager;

class CursorEventArgs : public InputEventArgs {
public:
  using Ptr = std::shared_ptr<CursorEventArgs>;

  static EventId ID;

  explicit CursorEventArgs() : InputEventArgs() {}
  virtual ~CursorEventArgs() {}

  virtual EventId GetEventId() override { return CursorEventArgs::ID; }

private:
  friend class InputManager;
};
} // namespace TwinkleGraphics

#endif