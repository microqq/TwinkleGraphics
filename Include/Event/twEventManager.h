#ifndef TW_EVENTMANAGER_H
#define TW_EVENTMANAGER_H

#include <array>
#include <map>
#include <mutex>

#include "twEvent.h"
#include "twEventArgs.h"
#include "twRingBuffer.h"
#include "twSingleton.h"

namespace TwinkleGraphics {
using EventQueue = RingBuffer<Event, 4096U>;
using MultiEventHandlerCollection = std::multimap<EventId, EventHandler>;

class __TWCOMExport EventManager : public IUpdatable,
                                   public INonCopyable,
                                   public IDestroyable {
public:
  virtual ~EventManager();
  virtual void Update(float deltaTime = 0.0f) override;
  virtual void Destroy() override {}

  void Subscribe(EventId id, const EventHandlerFunctionPtr &func);
  void Subscribe(EventId id, const EventHandler &handler);
  void UnSubscribe(EventId id, const EventHandlerFunctionPtr &func);
  void UnSubscribe(EventId id, const EventHandler &handler);

  void Fire(ObjectPtr sender, BaseEventArgsPtr args);
  void FireImmediately(ObjectPtr sender, BaseEventArgsPtr args);

  u32 GetEventsCount() { return _queue.Length(); }

private:
  explicit EventManager();

  EventHandler *FindFirstEventHandler(EventId id);
  void HandleEvents();
  void HandleEvents(ObjectPtr sender, BaseEventArgsPtr args);

  void CompressEvents();

private:
  EventQueue _queue;
  MultiEventHandlerCollection _handlerCollection;
#ifdef _EVT_MULTTHREAD
  std::mutex _queue_mutex;
  std::mutex _handlers_mutex;
#endif

  friend class Singleton<EventManager>;
};

__TWCOMExport EventManager &EventMgrInstance();
} // namespace TwinkleGraphics

#endif