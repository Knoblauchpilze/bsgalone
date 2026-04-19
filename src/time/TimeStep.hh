
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

  struct StepData
  {
    int ticks{};
    Duration duration{};
  };
  auto data() const -> StepData;

  auto count(const Duration &elapsed) const -> TickDuration;

  bool operator==(const TimeStep &rhs) const = default;

  private:
  int m_ticks{1};
  Duration m_duration{.unit = Unit::MILLISECONDS, .elapsed = 100.0f};

  friend auto operator<<(std::ostream &out, const TimeStep &step) -> std::ostream &;
  friend bool operator>>(std::istream &in, TimeStep &step);
};

auto operator<<(std::ostream &out, const TimeStep &step) -> std::ostream &;
bool operator>>(std::istream &in, TimeStep &step);

} // namespace chrono
