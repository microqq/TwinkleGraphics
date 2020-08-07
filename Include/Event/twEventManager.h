#ifndef TW_EVENTMANAGER_H
#define TW_EVENTMANAGER_H

#include "twSingleton.h"
#include "twRingBuffer.h"
#include "twEvent.h"

namespace TwinkleGraphics
{
    class EventManager;
    typedef RingBuffer<Event::Ptr> EventQueue;
    typedef Singleton<EventManager> EventManagerInst;

    class EventManager
    {
    public:
        EventManager();
        ~EventManager();

        void Subscribe(EventId id, EventHandler<BaseEventArgs> handler);
        void UnSubscribe(EventId id, EventHandler<BaseEventArgs> handler);

        void Fire(EventId id, BaseEventArgs::Ptr args);
        void FireImmediately(EventId id, BaseEventArgs::Ptr args);

        void Update();

    private:
        EventQueue _queue;
    };
} // namespace TwinkleGraphics

#endif