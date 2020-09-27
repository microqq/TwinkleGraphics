#ifndef TW_EVENTMANAGER_H
#define TW_EVENTMANAGER_H

#include <array>
#include <map>
#include <mutex>

#include "twSingleton.h"
#include "twRingBuffer.h"
#include "twEventArgs.h"
#include "twEvent.h"

namespace TwinkleGraphics
{
    typedef RingBuffer<Event> EventQueue;
    typedef std::multimap<EventId, EventHandler> MultiEventHandlerCollection;

    class __TWCOMExport EventManager : public IUpdatable
        , public INonCopyable
    {
    public:
        virtual ~EventManager();
        virtual void Update() override;

        void Subscribe(EventId id, const EventHandlerFunctionPtr& func);
        void Subscribe(EventId id, const EventHandler& handler);
        void UnSubscribe(EventId id, const EventHandlerFunctionPtr& func);
        void UnSubscribe(EventId id, const EventHandler& handler);

        void Fire(Object::Ptr sender, BaseEventArgs::Ptr args);
        void FireImmediately(Object::Ptr sender, BaseEventArgs::Ptr args);

    private:
        explicit EventManager();

        EventHandler* FindFirstEventHandler(EventId id);
        void HandleEvent();
        void HandleEvent(Object::Ptr sender, BaseEventArgs::Ptr args);

    private:
        EventQueue _queue;
        MultiEventHandlerCollection _handlerCollection;
#ifdef _EVT_MULTTHREAD
        std::mutex _queue_mutex;
        std::mutex _handlers_mutex;
#endif

        friend class Singleton<EventManager>;
    };

    __TWCOMExport EventManager& EventMgrInstance();
} // namespace TwinkleGraphics

#endif