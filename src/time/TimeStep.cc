
#include "TimeStep.hh"
#include "SerializationUtils.hh"

namespace chrono {

TimeStep::TimeStep(const int ticks, const Duration &inDuration)
  : m_ticks(ticks)
  , m_duration(inDuration)
{}

auto TimeStep::data() const -> StepData
{
  return StepData{
    .ticks    = m_ticks,
    .duration = m_duration,
  };
}

auto TimeStep::count(const Duration &elapsed) const -> TickDuration
{
  const auto in     = elapsed.convert(m_duration.unit);
  const auto epochs = in.elapsed / static_cast<float>(m_duration.elapsed);

  return TickDuration(epochs * static_cast<float>(m_ticks));
}

auto TimeStep::operator==(const TimeStep &rhs) const -> bool
{
  return (m_ticks == rhs.m_ticks) && (m_duration == rhs.m_duration);
}

auto operator<<(std::ostream &out, const TimeStep &step) -> std::ostream &
{
  core::serialize(out, step.m_ticks);
  core::serialize(out, step.m_duration.unit);
  core::serialize(out, step.m_duration.elapsed);

  return out;
}

bool operator>>(std::istream &in, TimeStep &step)
{
  bool ok{true};
  ok &= core::deserialize(in, step.m_ticks);
  ok &= core::deserialize(in, step.m_duration.unit);
  ok &= core::deserialize(in, step.m_duration.elapsed);

  return ok;
}

} // namespace chrono
