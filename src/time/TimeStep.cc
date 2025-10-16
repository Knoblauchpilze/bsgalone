
#include "TimeStep.hh"

namespace chrono {

TimeStep::TimeStep(const int ticks, const Duration &inDuration)
  : m_ticks(ticks)
  , m_duration(inDuration)
{}

auto TimeStep::count(const Duration &elapsed) const -> TickDuration
{
  const auto in     = elapsed.convert(m_duration.unit);
  const auto epochs = in.elapsed / static_cast<float>(m_duration.elapsed);

  return TickDuration(epochs * static_cast<float>(m_ticks));
}

} // namespace chrono
