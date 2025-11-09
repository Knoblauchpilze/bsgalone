
#include "TimeStepUtils.hh"

namespace pge {

auto convertTickToSeconds(const chrono::TickDuration &elapsed, const chrono::TimeStep &timeStep)
  -> float
{
  const auto oneSecond        = chrono::Duration::fromSeconds(1.0f);
  const auto ticksInOneSecond = timeStep.count(oneSecond);

  // - elapsed has a dimension of: [Tick]
  // - ticksInOnSecond as a dimension of: [Tick][Second]^-1
  // The result of the division has a dimension of:
  // [Tick] / ([Tick][Second]^-1) = [Second]
  return elapsed / ticksInOneSecond;
}

} // namespace pge
