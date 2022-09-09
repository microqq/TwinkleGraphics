#ifndef TW_EVENTARGS_H
#define TW_EVENTARGS_H

#include <functional>

#include "twCommon.h"
#include "twObject.h"

#define DEFINE_EVENTID(T) EventId T::ID = std::hash<std::string>{}(#T)

namespace TwinkleGraphics {
using EventId = unsigned int;

enum EventType {
  UNKNOWN = 0,

  TIMER = 1,

  MOUSEPRESS = 2,
  MOUSERELEASE = 3,
  MOUSEMOVE = 4,
  MOUSEDOUBLECLICK = 5,

  KEYPRESS = 6,
  KEYRELEASE = 7,

  FOCUSIN = 8,
  FOCUSOUT = 9,

  ENTER = 10,
  LEAVE = 11,

  MOVE = 12,
  RESIZE = 13,
  CLOSE = 14,

  WHEEL = 15,

  QUIT = 16
};

class __TWCOMExport BaseEventArgs : public Object {
public:
  using Ptr = std::shared_ptr<BaseEventArgs>;

  BaseEventArgs() : Object() {}
  virtual ~BaseEventArgs() = 0;

  virtual EventId GetEventId() = 0;

  void SetCancelFlag(bool flag) { _cancelFlag = flag; }
  bool GetCancelFlag() { return _cancelFlag; }

protected:
  bool _cancelFlag = false;
  bool _shouldBeCompressed = false;
};

using BaseEventArgsPtr = BaseEventArgs::Ptr ;
} // namespace TwinkleGraphics

#endif