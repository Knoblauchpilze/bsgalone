
#pragma once

#include "IEventListener.hh"
#include <memory>

namespace messaging {

template<EventTypeLike EventType, EventLike<EventType> Event>
class IEventQueue
{
  public:
  using EventPtr         = std::unique_ptr<Event>;
  using EventListener    = IEventListener<EventType, Event>;
  using EventListenerPtr = std::unique_ptr<EventListener>;

  virtual ~IEventQueue() = default;

  virtual void pushEvent(EventPtr message)            = 0;
  virtual void addListener(EventListenerPtr listener) = 0;
  virtual bool empty()                                = 0;

  virtual void processEvents() = 0;
};

} // namespace messaging
