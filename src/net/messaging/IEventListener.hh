

#pragma once

#include "IEvent.hh"
#include <memory>

namespace net {

class IEventListener
{
  public:
  virtual ~IEventListener() = default;

  virtual bool isEventRelevant(const EventType &type) const = 0;
  virtual void onEventReceived(const IEvent &event)         = 0;
};

using IEventListenerPtr = std::unique_ptr<IEventListener>;

} // namespace net
