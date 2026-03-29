
#pragma once

#include "IGameEvent.hh"
#include <memory>

namespace bsgalone::core {

class ForPublishingEvent
{
  public:
  ForPublishingEvent()          = default;
  virtual ~ForPublishingEvent() = default;

  virtual void publishEvent(IGameEventPtr event) = 0;
};

using ForPublishingEventShPtr = std::shared_ptr<ForPublishingEvent>;

} // namespace bsgalone::core
