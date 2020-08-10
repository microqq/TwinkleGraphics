#ifndef TW_EVENTMANAGER_H
#define TW_EVENTMANAGER_H

#include <array>
#include <map>

#include "twSingleton.h"
#include "twRingBuffer.h"
#include "twEventArgs.h"
#include "twEvent.h"

namespace TwinkleGraphics
{
    class EventManager;
    typedef Singleton<EventManager> EventManagerInst;
    // typedef RingBuffer<Event::Ptr> EventQueue;
    typedef std::array<Event::Ptr, 1000> EventQueue;
    typedef std::multimap<EventId, EventHandler> MultiEventHandlerCollection;

    class EventManager
    {
    public:
        EventManager();
        ~EventManager();

        void Subscribe(EventId id, EventHandler::HandlerFunc func);
        void Subscribe(EventId id, EventHandler::HandlerFuncPtr funcPtr);
        void Subscribe(EventId id, EventHandler::Ptr handler);
        void UnSubscribe(EventId id, EventHandler::Ptr handler);

        void Fire(EventId id, BaseEventArgs::Ptr args);
        void FireImmediately(EventId id, BaseEventArgs::Ptr args);

        void Update();

    private:
        void HandleEvent(EventId id);

    private:
        EventQueue _queue;
        MultiEventHandlerCollection _handlerCollection;
    };
} // namespace TwinkleGraphics

#endif