#ifndef TW_EVENT_HPP
#define TW_EVENT_HPP

#include <utility>

#include "twObject.h"
#include "twEventArgs.h"
#include "twEventHandler.h"

namespace TwinkleGraphics
{

    class Event : public Object
    {
    public:
        typedef std::shared_ptr<Event> Ptr;

        Event(Object::Ptr sender, BaseEventArgs::Ptr args)
            : Object()
            , _sender(sender)
            , _eventArgs(args)
        {
        }

        Event(const Event &src)
            : Object()
        {
            _sender = src._sender;
            _eventArgs = src._args;
        }

        Event(Event &&src)
            : Object()
        {
            std::swap(_sender, src._sender);     
            std::swap(_eventArgs, src._eventArgs);     
        }

        virtual ~Event()
        {
            _sender = nullptr;
            _eventArgs = nullptr;
        }

    private:
        Object::Ptr _sender;
        BaseEventArgs::Ptr _eventArgs;
    };
} // namespace TwinkleGraphics

#endif