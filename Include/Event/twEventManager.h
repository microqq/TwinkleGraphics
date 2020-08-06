#ifndef TW_EVENTMANAGER_H
#define TW_EVENTMANAGER_H

#include "twSingleton.h"
#include "twRingBuffer.h"
#include "twEvent.h"

namespace TwinkleGraphics
{
    class EventManager;
    typedef RingBuffer<Event<int>> EventQueue;
    typedef Singleton<EventManager> ShaderManagerInst;

    class EventManager
    {
    public:
        EventManager();
        ~EventManager();

    private:
        EventQueue _queue;
    };
} // namespace TwinkleGraphics

#endif