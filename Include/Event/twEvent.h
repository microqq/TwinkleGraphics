#ifndef TW_EVENT_H
#define TW_EVENT_H

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

        Event()
            : Object()
        {}

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
            _eventArgs = src._eventArgs;
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

        Event& operator=(const Event& src)
        {
            _sender = src._sender;
            _eventArgs = src._eventArgs;

            return *this;
        }

        Event& operator=(Event&& src)
        {
            std::swap(_sender, src._sender);     
            std::swap(_eventArgs, src._eventArgs);     

            return *this;
        }

        Object::Ptr Sender() { return _sender; }
        BaseEventArgs::Ptr EventArgs() { return _eventArgs; }
        void SetSender(Object::Ptr sender) { _sender = sender; }
        void SetEventArgs(BaseEventArgs::Ptr args) { _eventArgs = args; }

    private:
        Object::Ptr _sender;
        BaseEventArgs::Ptr _eventArgs;
    };
} // namespace TwinkleGraphics

#endif