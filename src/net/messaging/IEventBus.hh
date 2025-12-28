
#pragma once

#include "CoreObject.hh"
#include "IEvent.hh"
#include "IEventListener.hh"
#include <memory>

namespace net {

class IEventBus
{
  public:
  virtual ~IEventBus() = default;

  virtual void pushEvent(IEventPtr event)              = 0;
  virtual void addListener(IEventListenerPtr listener) = 0;
  virtual bool empty()                                 = 0;

  virtual void processEvents() = 0;

  private:
  std::vector<IEventListenerPtr> m_listeners{};
  std::unordered_multimap<EventType, IEventListener *> m_listenersTable{};
};

using IEventBusPtr = std::unique_ptr<IEventBus>;

} // namespace net
