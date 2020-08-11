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
    typedef RingBuffer<Event> EventQueue;
    typedef std::multimap<EventId, EventHandler> MultiEventHandlerCollection;

    class EventManager
    {
    public:
        EventManager();
        ~EventManager();

        void Subscribe(EventId id, EventHandler::HandlerFunc func);
        void Subscribe(EventId id, EventHandler::HandlerFuncPtr funcPtr);
        void Subscribe(EventId id, const EventHandler& handler);
        void UnSubscribe(EventId id, const EventHandler::HandlerFunc& func);
        void UnSubscribe(EventId id, const EventHandler::HandlerFuncPtr& funcPtr);
        void UnSubscribe(EventId id, const EventHandler& handler);

        void Fire(Object::Ptr sender, BaseEventArgs::Ptr args);
        void FireImmediately(Object::Ptr sender, BaseEventArgs::Ptr args);

        void Update();

    private:
        EventHandler* FindFirstEventHandler(EventId id);
        void HandleEvent(Object::Ptr sender, BaseEventArgs::Ptr args);

    private:
        EventQueue _queue;
        MultiEventHandlerCollection _handlerCollection;
    };
} // namespace TwinkleGraphics

#endif