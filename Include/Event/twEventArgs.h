#ifndef TW_EVENTARGS_H
#define TW_EVENTARGS_H

#include <functional>

#include "twObject.h"

namespace TwinkleGraphics
{
    typedef unsigned int EventId;

    class BaseEventArgs : public Object
    {
    public:
        typedef std::shared_ptr<BaseEventArgs> Ptr;

        BaseEventArgs() : Object() {}
        virtual ~BaseEventArgs() = 0;

        inline EventId GetEventId() 
        { 
            if(-1 == _eventid)
            {
                _eventid = std::hash<std::string>{}(typeid(*this).name());
            }
            return _eventid; 
        }

    protected:
        EventId _eventid = -1;
    };
} // namespace TwinkleGraphics


#endif