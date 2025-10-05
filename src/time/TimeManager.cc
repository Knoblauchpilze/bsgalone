
#include "TimeManager.hh"
#include <cmath>

namespace chrono {

TimeManager::TimeManager(const bsgo::Tick tick, const TimeStep step)
  : core::CoreObject("manager")
  , m_currentTick(tick)
  , m_step(step)
{
  setService("time");
}

auto TimeManager::tick(const Duration elapsed) -> bsgo::TickData
{
  const auto tick = m_step.count(elapsed);
  m_currentTick += tick;

  verbose(elapsed.str() + " elapsed, tick: " + m_currentTick.str() + " (delta: " + tick.str() + ")");

  return bsgo::TickData{
    .tick    = m_currentTick,
    .elapsed = tick,
  };
}

} // namespace chrono
