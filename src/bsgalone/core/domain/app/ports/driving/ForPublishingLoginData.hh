
#pragma once

#include "Uuid.hh"
#include <memory>

namespace bsgalone::core {

class ForPublishingLoginData
{
  public:
  ForPublishingLoginData()          = default;
  virtual ~ForPublishingLoginData() = default;

  virtual void publishLoginData(const Uuid playerDbId) = 0;
};

using ForPublishingLoginDataPtr = std::unique_ptr<ForPublishingLoginData>;

} // namespace bsgalone::core
