
#pragma once

#include "TickData.hh"
#include <memory>

namespace bsgalone::core {

class ForManagingHealth
{
  public:
  virtual ~ForManagingHealth() = default;

  virtual void updateHealth(const chrono::TickData &data) = 0;
};

using ForManagingHealthPtr = std::unique_ptr<ForManagingHealth>;

} // namespace bsgalone::core
