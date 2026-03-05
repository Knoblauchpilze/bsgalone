
#include "TimeStepUtils.hh"

namespace pge {

auto convertTickToDuration(const chrono::TickDuration &elapsed, const chrono::TimeStep &timeStep)
  -> core::Duration
{
  const auto oneSecond        = chrono::Duration::fromSeconds(1.0f);
  const auto ticksInOneSecond = timeStep.count(oneSecond);

  // - elapsed has a dimension of: [Tick]
  // - ticksInOnSecond as a dimension of: [Tick][Second]^-1
  // The result of the division has a dimension of:
  // [Tick] / ([Tick][Second]^-1) = [Second]
  // The dimensionless constant converts from seconds to milliseconds.
  constexpr auto MILLIS_IN_ONE_SECOND = 1000.0f;
  const auto ms                       = MILLIS_IN_ONE_SECOND * (elapsed / ticksInOneSecond);

  // A bit of precision is lost here but it's below a millisecond. This is only
  // availale in the client and used for the UI so this is fine.
  return core::fromMilliseconds(ms);
}

} // namespace pge
