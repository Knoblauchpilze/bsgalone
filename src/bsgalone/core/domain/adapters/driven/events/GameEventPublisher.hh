
#pragma once

#include "ForPublishingEvent.hh"

namespace bsgalone::core {

class GameEventPublisher : public ForPublishingEvent
{
  public:
  GameEventPublisher()           = default;
  ~GameEventPublisher() override = default;

  void publishEvent(IGameEventPtr event) override;
};

} // namespace bsgalone::core
