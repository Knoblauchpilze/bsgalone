
#pragma once

#include "EventConcept.hh"
#include "EventTypeConcept.hh"

namespace messaging {

template<EventTypeLike EventType, EventLike<EventType> Event>
class IEventListener
{
  public:
  virtual ~IEventListener() = default;

  virtual bool isEventRelevant(const EventType &type) const = 0;
  virtual void onEventReceived(const Event &event)          = 0;
};

} // namespace messaging
