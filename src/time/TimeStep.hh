
#pragma once

#include "Duration.hh"
#include "TickDuration.hh"

namespace chrono {

class TimeStep
{
  public:
  TimeStep() = default;
  TimeStep(const int ticks, const Duration &inDuration);
  ~TimeStep() = default;

  auto count(const Duration &elapsed) const -> TickDuration;

  private:
  int m_ticks{1};
  Duration m_duration{.unit = Unit::MILLISECONDS, .elapsed = 100.0f};
};

} // namespace chrono