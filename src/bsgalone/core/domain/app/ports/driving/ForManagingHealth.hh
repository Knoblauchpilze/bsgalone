
#pragma once

#include "TickData.hh"

namespace bsgalone::core {

class ForManagingHealth
{
  public:
  virtual ~ForManagingHealth() = default;

  virtual void updateHealth(const chrono::TickData &data) = 0;
};

} // namespace bsgalone::core
