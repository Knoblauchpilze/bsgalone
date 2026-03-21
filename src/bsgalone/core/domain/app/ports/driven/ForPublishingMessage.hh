
#pragma once

#include "IGameEvent.hh"
#include <memory>

namespace bsgalone::core {

class ForPublishingMessage
{
  public:
  ForPublishingMessage()          = default;
  virtual ~ForPublishingMessage() = default;

  virtual void publishEvent(IGameEventPtr event) = 0;
};

using ForPublishingMessageShPtr = std::shared_ptr<ForPublishingMessage>;

} // namespace bsgalone::core
