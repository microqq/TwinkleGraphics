#ifndef TW_FOCUSEVENTARGS_H
#define TW_FOCUSEVENTARGS_H

#include "twInputEventArgs.h"

namespace TwinkleGraphics {
class InputManager;

class FocusEventArgs : public InputEventArgs {
public:
  using Ptr = std::shared_ptr<FocusEventArgs>;

  static EventId ID;

  explicit FocusEventArgs() : InputEventArgs() {}
  virtual ~FocusEventArgs() {}

  virtual EventId GetEventId() override { return FocusEventArgs::ID; }

private:
  friend class InputManager;
};
} // namespace TwinkleGraphics

#endif