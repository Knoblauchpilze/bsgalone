
#pragma once

#include "CoreObject.hh"
#include "Duration.hh"
#include "Tick.hh"
#include "TickData.hh"
#include "TimeStep.hh"
#include <memory>

namespace chrono {

class TimeManager : public core::CoreObject
{
  public:
  TimeManager(const Tick tick, const TimeStep step);
  ~TimeManager() = default;

  auto tick(const Duration elapsed) -> TickData;

  private:
  Tick m_currentTick{};
  TimeStep m_step{};
};

using TimeManagerPtr = std::unique_ptr<TimeManager>;

} // namespace chrono
