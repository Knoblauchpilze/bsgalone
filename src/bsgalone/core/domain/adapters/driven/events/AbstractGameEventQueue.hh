
#pragma once

#include "ForProcessingEvents.hh"
#include "ForPublishingEvent.hh"
#include "IGameEvent.hh"
#include "IGameEventListener.hh"
#include <unordered_map>
#include <vector>

namespace bsgalone::core {

class AbstractGameEventQueue : public ForPublishingEvent, public ForProcessingEvents
{
  public:
  AbstractGameEventQueue()           = default;
  ~AbstractGameEventQueue() override = default;

  void addListener(IGameEventListenerPtr listener) override;

  protected:
  std::vector<IGameEventListenerPtr> m_listeners{};
  std::unordered_multimap<GameEventType, IGameEventListener *> m_listenersTable{};
};

} // namespace bsgalone::core
