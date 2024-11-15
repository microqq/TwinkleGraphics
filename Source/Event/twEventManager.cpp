
#include "twEventManager.h"

namespace TwinkleGraphics {
EventManager &EventMgrInstance() { return Singleton<EventManager>::Instance(); }

EventManager::EventManager()
    : IUpdatable(), INonCopyable(), IDestroyable(), _queue(),
      _handlerCollection() {}

EventManager::~EventManager() {
  {
#ifdef _EVT_MULTTHREAD
    std::lock_guard<std::mutex> lock(_handlers_mutex);
#endif
    _handlerCollection.clear();
  }
}

/**
 *
 */
void EventManager::Subscribe(EventId id, const EventHandlerFunctionPtr &func) {
#ifdef _EVT_MULTTHREAD
  std::lock_guard<std::mutex> lock(_handlers_mutex);
#endif
  using MIterator = MultiEventHandlerCollection::iterator;
  MIterator iter = _handlerCollection.lower_bound(id);
  MIterator end = _handlerCollection.upper_bound(id);

  while (iter != end) {
    if (*(iter->second.FindHandlerFunc(func)) != nullptr) {
      return;
    }
    ++iter;
  }

  EventHandler h(func);
  _handlerCollection.insert(std::make_pair(id, h));
}

/**
 * Duplicate handler are not allowed.
 */
void EventManager::Subscribe(EventId id, const EventHandler &handler) {
#ifdef _EVT_MULTTHREAD
  std::lock_guard<std::mutex> lock(_handlers_mutex);
#endif

  using MIterator = MultiEventHandlerCollection::iterator;
  MIterator iter = _handlerCollection.lower_bound(id);
  MIterator end = _handlerCollection.upper_bound(id);

  while (iter != end) {
    if (iter->second == handler) {
      return;
    }
    ++iter;
  }

  _handlerCollection.insert(std::make_pair(id, handler));
}

void EventManager::UnSubscribe(EventId id,
                               const EventHandlerFunctionPtr &func) {
#ifdef _EVT_MULTTHREAD
  std::lock_guard<std::mutex> lock(_handlers_mutex);
#endif

  using MIterator = MultiEventHandlerCollection::iterator;
  MIterator iter = _handlerCollection.lower_bound(id);
  MIterator end = _handlerCollection.upper_bound(id);

  while (iter != end) {
    if (*(iter->second.FindHandlerFunc(func)) != nullptr) {
      break;
    }
    ++iter;
  }

  if (iter != end) {
    _handlerCollection.erase(iter);
  }
}

void EventManager::UnSubscribe(EventId id, const EventHandler &handler) {
#ifdef _EVT_MULTTHREAD
  std::lock_guard<std::mutex> lock(_handlers_mutex);
#endif

  using MIterator = MultiEventHandlerCollection::iterator;
  MIterator iter = _handlerCollection.lower_bound(id);
  MIterator end = _handlerCollection.upper_bound(id);

  while (iter != end) {
    if (iter->second == handler) {
      break;
    }
    ++iter;
  }

  if (iter != end) {
    _handlerCollection.erase(iter);
  }
}

void EventManager::Fire(ObjectPtr sender, BaseEventArgsPtr args) {
  if (args == nullptr) {
    return;
  }

#ifdef _EVT_MULTTHREAD
  std::lock_guard<std::mutex> lock(_queue_mutex);
#endif
  // Create Event, push back into event queue
  Event *event = _queue.PushBack();
  event->SetSender(sender);
  event->SetEventArgs(args);
}

void EventManager::FireImmediately(ObjectPtr sender, BaseEventArgsPtr args) {
  // HandlerEvent
  HandleEvents(sender, args);
}

void EventManager::Update(float deltaTime) {
#ifdef _DEBUG
  // u32 count = GetEventsCount();
  // if(count > 0)
  // {
  //     Console::LogInfo("Events Count:", GetEventsCount(), "\n");
  // }
#endif
  // Pop event and handle it
  HandleEvents();
}

EventHandler *EventManager::FindFirstEventHandler(EventId id) {
  MultiEventHandlerCollection::iterator begin =
      _handlerCollection.lower_bound(id);
  MultiEventHandlerCollection::iterator end =
      _handlerCollection.upper_bound(id);

  if (begin != end) {
    return &(begin->second);
  }

  return nullptr;
}

void EventManager::HandleEvents() {
  Event *event = nullptr;
  {
#ifdef _EVT_MULTTHREAD
    std::lock_guard<std::mutex> lock(_queue_mutex);
#endif
    while ((event = _queue.PopFront()) != nullptr) {
      HandleEvents(event->Sender(), event->EventArgs());
    }
  }
}

void EventManager::HandleEvents(ObjectPtr sender, BaseEventArgsPtr args) {
  if (args == nullptr) {
    return;
  }

  EventId id = args->GetEventId();
  {
#ifdef _EVT_MULTTHREAD
    std::lock_guard<std::mutex> lock(_handlers_mutex);
#endif
    using MIterator = MultiEventHandlerCollection::iterator;
    MIterator iter = _handlerCollection.lower_bound(id);
    MIterator end = _handlerCollection.upper_bound(id);

    while (iter != end) {
      iter->second.Invoke(sender, args);
      ++iter;
    }
  }
}

void EventManager::CompressEvents() {}
} // namespace TwinkleGraphics
