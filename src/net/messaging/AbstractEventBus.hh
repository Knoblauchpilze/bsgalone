
#pragma once

#include "IEvent.hh"
#include "IEventBus.hh"
#include "IEventListener.hh"
#include <unordered_map>
#include <vector>

namespace net {

class AbstractEventBus : public IEventBus
{
  public:
  AbstractEventBus()           = default;
  ~AbstractEventBus() override = default;

  void addListener(IEventListenerPtr listener) override;

  protected:
  std::vector<IEventListenerPtr> m_listeners{};
  std::unordered_multimap<EventType, IEventListener *> m_listenersTable{};
};

} // namespace net
