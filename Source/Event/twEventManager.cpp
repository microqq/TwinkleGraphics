
#include "twEventManager.h"

namespace TwinkleGraphics
{

    EventManager::EventManager()
    {
    }

    EventManager::~EventManager()
    {
    }

    void EventManager::Subscribe(EventId id, EventHandler::HandlerFunc func)
    {}

    void EventManager::Subscribe(EventId id, EventHandler::HandlerFuncPtr funcPtr)
    {
    }

    void EventManager::Subscribe(EventId id, EventHandler::Ptr handler)
    {
    }
    void EventManager::UnSubscribe(EventId id, EventHandler::Ptr handler)
    {
    }

    void EventManager::Fire(EventId id, BaseEventArgs::Ptr args)
    {
    }
    
    void EventManager::FireImmediately(EventId id, BaseEventArgs::Ptr args)
    {
    }

    void EventManager::Update()
    {
    }

} // namespace TwinkleGraphics
