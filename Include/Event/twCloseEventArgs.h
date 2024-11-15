#ifndef TW_CLOSEEVENTARGS_H
#define TW_CLOSEEVENTARGS_H

#include "twInputEventArgs.h"

namespace TwinkleGraphics {
class InputManager;

class CloseEventArgs : public InputEventArgs {
public:
  using Ptr = std::shared_ptr<CloseEventArgs>;

  static EventId ID;

  explicit CloseEventArgs() : InputEventArgs() {}
  virtual ~CloseEventArgs() {}

  virtual EventId GetEventId() override { return CloseEventArgs::ID; }

private:
  friend class InputManager;
};
} // namespace TwinkleGraphics

#endif