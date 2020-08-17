
#include "twEventManager.h"

namespace TwinkleGraphics
{

    EventManager::EventManager()
        : _queue()
        , _handlerCollection()
    {
    }

    EventManager::~EventManager()
    {
        _handlerCollection.clear();
    }

    /**
     *   
     */
    void EventManager::Subscribe(EventId id, const HandlerFunctionPtr& func)
    {
        using MIterator = MultiEventHandlerCollection::iterator;
        MIterator iter = _handlerCollection.lower_bound(id);
        MIterator end = _handlerCollection.upper_bound(id);

        while(iter != end)
        {
            if(*(iter->second.FindHandlerFunc(func)) != nullptr)
            {
                break;
            }
            ++iter;
        }
        
        EventHandler h(func);
        _handlerCollection.insert(std::make_pair(id, h));
    }

    /**
     * Duplicate handler are not allowed.
     */
    void EventManager::Subscribe(EventId id, const EventHandler& handler)
    {
        using MIterator = MultiEventHandlerCollection::iterator;
        MIterator iter = _handlerCollection.lower_bound(id);
        MIterator end = _handlerCollection.upper_bound(id);

        while(iter != end)
        {
            if(iter->second == handler)
            {
                return;
            }
            ++iter;
        }

        _handlerCollection.insert(std::make_pair(id, handler));
    }

    void EventManager::UnSubscribe(EventId id, const HandlerFunctionPtr& func)
    {
        using MIterator = MultiEventHandlerCollection::iterator;
        MIterator iter = _handlerCollection.lower_bound(id);
        MIterator end = _handlerCollection.upper_bound(id);

        while(iter != end)
        {
            if(*(iter->second.FindHandlerFunc(func)) != nullptr)
            {
                break;
            }
            ++iter;
        }

        if(iter != end)
        {
            _handlerCollection.erase(iter);
        }
    }

    void EventManager::UnSubscribe(EventId id, const EventHandler& handler)
    {
        using MIterator = MultiEventHandlerCollection::iterator;
        MIterator iter = _handlerCollection.lower_bound(id);
        MIterator end = _handlerCollection.upper_bound(id);

        while(iter != end)
        {
            if(iter->second == handler)
            {
                break;
            }
            ++iter;
        }

        if(iter != end)
        {
            _handlerCollection.erase(iter);
        }
    }

    void EventManager::Fire(Object::Ptr sender, BaseEventArgs::Ptr args)
    {
        if(args == nullptr)
        {
            return;
        }

        // Create Event, push back into event queue
        Event* event = _queue.PushBack();
        event->SetSender(sender);
        event->SetEventArgs(args);
    }
    
    void EventManager::FireImmediately(Object::Ptr sender, BaseEventArgs::Ptr args)
    {
        // HandlerEvent
        HandleEvent(sender, args);
    }

    void EventManager::Update()
    {
        // Pop event and handle it


    }

    EventHandler* EventManager::FindFirstEventHandler(EventId id)
    {
        MultiEventHandlerCollection::iterator begin = _handlerCollection.lower_bound(id);
        MultiEventHandlerCollection::iterator end = _handlerCollection.upper_bound(id);

        if(begin != end)
        {
            return &(begin->second);
        }

        return nullptr;
    }

    void EventManager::HandleEvent(Object::Ptr sender, BaseEventArgs::Ptr args)
    {
        if(args == nullptr)
        {
            return;
        }

        EventId id = args->GetEventId();        
        using MIterator = MultiEventHandlerCollection::iterator;
        MIterator iter = _handlerCollection.lower_bound(id);
        MIterator end = _handlerCollection.upper_bound(id);

        while(iter != end)
        {
            iter->second.Invoke(sender, args);
            ++iter;
        }
    }
} // namespace TwinkleGraphics
