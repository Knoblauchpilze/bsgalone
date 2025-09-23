
#pragma once

#include "CoreObject.hh"
#include "Duration.hh"
#include "Tick.hh"
#include "TimeStep.hh"

namespace chrono {

class TimeManager : public core::CoreObject
{
  public:
  TimeManager(const Tick tick, const TimeStep step);
  ~TimeManager() = default;

  auto tick(const Duration elapsed) -> Tick;

  private:
  Tick m_currentTick{};
  TimeStep m_step{};
};

} // namespace chrono
