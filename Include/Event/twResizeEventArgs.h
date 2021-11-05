#ifndef TW_RESIZEEVENTARGS_H
#define TW_RESIZEEVENTARGS_H

#include "twInputEventArgs.h"

namespace TwinkleGraphics {
class InputManager;

class ResizeEventArgs : public InputEventArgs {
public:
  typedef std::shared_ptr<ResizeEventArgs> Ptr;

  static EventId ID;

  explicit ResizeEventArgs() : InputEventArgs() {}
  virtual ~ResizeEventArgs() {}

  virtual EventId GetEventId() override { return ResizeEventArgs::ID; }

  void SetOldSize(ivec2 size) { _oldSize = size; }
  void SetSize(ivec2 size) { _size = size; }

  ivec2 OldSize() { return _oldSize; }
  ivec2 Size() { return _size; }

private:
  ivec2 _oldSize;
  ivec2 _size;

  friend class InputManager;
};
} // namespace TwinkleGraphics

#endif