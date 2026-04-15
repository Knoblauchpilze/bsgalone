
#pragma once

#include "Duration.hh"
#include "TickData.hh"
#include "TimeStep.hh"
#include <memory>

namespace chrono {

class ITimeManager
{
  public:
  virtual ~ITimeManager() = default;

  virtual auto tick(const Duration elapsed) -> TickData = 0;
};

using ITimeManagerPtr = std::unique_ptr<ITimeManager>;

} // namespace chrono
