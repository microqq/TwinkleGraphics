#ifndef TW_EVENTARGS_H
#define TW_EVENTARGS_H

#include <functional>

#include "twCommon.h"
#include "twObject.h"

#define DEFINE_EVENTID(T) EventId T::ID = \
                               std::hash<std::string>{}(#T)

namespace TwinkleGraphics
{
    typedef unsigned int EventId;

    enum EventType
    {
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

    class __TWCOMExport BaseEventArgs : public Object
    {
    public:
        typedef std::shared_ptr<BaseEventArgs> Ptr;

        BaseEventArgs() : Object() {}
        virtual ~BaseEventArgs() = 0;

        virtual EventId GetEventId() = 0;

    protected:
    };
} // namespace TwinkleGraphics


#endif