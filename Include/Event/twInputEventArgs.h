#ifndef TW_INPUTEVENTARGS_H
#define TW_INPUTEVENTARGS_H

#include "twEventArgs.h"

namespace TwinkleGraphics {
class InputManager;

class InputEventArgs : public BaseEventArgs {
public:
  typedef std::shared_ptr<InputEventArgs> Ptr;

  static EventId ID;

  explicit InputEventArgs() : BaseEventArgs() {}
  virtual ~InputEventArgs() {}
  virtual EventId GetEventId() override { return InputEventArgs::ID; }

private:
  friend class InputManager;
};
} // namespace TwinkleGraphics

#endif