
#pragma once

#include "CoreObject.hh"
#include "ITimeManager.hh"

namespace chrono {

class TimeManager : public ITimeManager, public core::CoreObject
{
  public:
  TimeManager(const Tick tick, const TimeStep step);
  ~TimeManager() = default;

  auto tick(const Duration elapsed) -> TickData override;

  private:
  Tick m_currentTick{};
  TimeStep m_step{};
};

} // namespace chrono
