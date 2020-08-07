#ifndef TW_EVENTARGS_H
#define TW_EVENTARGS_H

#include "twObject.h"

namespace TwinkleGraphics
{
    typedef unsigned int EventId;

    class BaseEventArgs : public Object
    {
    public:
        typedef std::shared_ptr<BaseEventArgs> Ptr;

        BaseEventArgs() : Object() {}
        virtual ~BaseEventArgs() {}

        // virtual EventId GetEventId() = 0;
        virtual EventId GetEventId() { return 0; }
    };    
} // namespace TwinkleGraphics


#endif