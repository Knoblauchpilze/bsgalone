
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

auto TimeManager::tick(const Duration elapsed) -> Tick
{
  const auto tick = m_step.count(elapsed);
  m_currentTick += tick;

  debug(elapsed.str() + " elapsed, tick: " + m_currentTick.str() + " (delta: " + tick.str() + ")");

  return m_currentTick;
}

} // namespace chrono
