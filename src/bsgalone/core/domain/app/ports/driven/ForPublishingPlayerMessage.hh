
#pragma once

#include "System.hh"
#include "Uuid.hh"
#include <memory>
#include <vector>

namespace bsgalone::core {

class ForPublishingPlayerMessage
{
  public:
  ForPublishingPlayerMessage()          = default;
  virtual ~ForPublishingPlayerMessage() = default;

  virtual void publishSystemList(const Uuid playerDbId, const std::vector<System> &systems) = 0;
};

using ForPublishingPlayerMessageShPtr = std::shared_ptr<ForPublishingPlayerMessage>;

} // namespace bsgalone::core
