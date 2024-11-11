#ifndef TW_EVENT_H
#define TW_EVENT_H

#include "twEventArgs.h"
#include "twEventHandler.h"
#include "twObject.h"

namespace TwinkleGraphics {
class __TWAPI Event : public Object {
public:
  using Ptr = std::shared_ptr<Event>;

  Event() : Object() {}

  Event(ObjectPtr sender, BaseEventArgsPtr args)
      : Object(), _sender(sender), _eventArgs(args) {}

  Event(const Event &src) : Object() {
    _sender = src._sender;
    _eventArgs = src._eventArgs;
  }

  Event(Event &&src) : Object() {
    std::swap(_sender, src._sender);
    std::swap(_eventArgs, src._eventArgs);
  }

  virtual ~Event() {
    _sender = nullptr;
    _eventArgs = nullptr;
  }

  Event &operator=(const Event &src) {
    _sender = src._sender;
    _eventArgs = src._eventArgs;

    return *this;
  }

  Event &operator=(Event &&src) {
    std::swap(_sender, src._sender);
    std::swap(_eventArgs, src._eventArgs);

    return *this;
  }

  ObjectPtr Sender() { return _sender; }
  BaseEventArgsPtr EventArgs() { return _eventArgs; }
  void SetSender(ObjectPtr sender) { _sender = sender; }
  void SetEventArgs(BaseEventArgsPtr args) { _eventArgs = args; }

private:
  ObjectPtr _sender;
  BaseEventArgsPtr _eventArgs;
};

using EventPtr = Event::Ptr;

} // namespace TwinkleGraphics

#endif