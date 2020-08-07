#ifndef TW_EVENTHANDLER_HPP
#define TW_EVENTHANDLER_HPP

#include <functional>
#include <list>

#include "twObject.h"
#include "twEventArgs.h"

namespace TwinkleGraphics
{
    template<class T>
    class EventHandler : public Object
    {
    public:
        typedef std::function<void(Object::Ptr, T)> HandlerFunc;
        typedef std::shared_ptr<EventHandler<T>> Ptr;

        EventHandler(const HandlerFunc& func)
            : Object()
        {
            _handlerFuncList.insert(_handlerFuncList.end(), func);
        }

        EventHandler(const EventHandler &src)
            : Object()
        {
        }

        EventHandler(EventHandler &&src)
            : Object()
        {
        }

        virtual ~EventHandler()
        {
        }

        EventHandler& operator=(const EventHandler& src)
        {
            return *this;
        }

        EventHandler& operator=(const EventHandler&& src)
        {
            return *this;
        }

        EventHandler& operator+=(HandlerFunc& handler)
        {
            _handlerFuncList.insert(_handlerFuncList.end(), handler);
            return *this;
        }

        EventHandler& operator-=(HandlerFunc& handler)
        {
            return *this;
        }

        void Invoke(Object::Ptr sender, T args)
        {
            for(auto handler : _handlerFuncList)
            {
                handler(sender, args);
            }
        }

    private:
        void Add(HandlerFunc& handler);
        void Remove(HandlerFunc& handler);

    private:
        std::list<HandlerFunc> _handlerFuncList;
        EventId _eventId;
    };
} // namespace TwinkleGraphics

#endif