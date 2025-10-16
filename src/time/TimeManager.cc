
#include "TimeManager.hh"
#include <cmath>

namespace chrono {

TimeManager::TimeManager(const Tick tick, const TimeStep step)
  : core::CoreObject("manager")
  , m_currentTick(tick)
  , m_step(step)
{
  setService("time");
}

auto TimeManager::tick(const Duration elapsed) -> TickData
{
  const auto duration = m_step.count(elapsed);
  m_currentTick += duration;

  verbose(elapsed.str() + " elapsed, tick: " + m_currentTick.str() + " (delta: " + duration.str()
          + ")");

  return TickData{
    .tick    = m_currentTick,
    .elapsed = duration,
  };
}

} // namespace chrono
