
#include "TickDuration.hh"
#include <cmath>
#include <stdexcept>

namespace bsgo {

// Using this value based on powers of two to accurately represent the threshold.
// This is equivalent to about 0.00000762939.
const float TickDuration::TOLERANCE = std::ldexp(1.0f, -17);

TickDuration::TickDuration(const float elapsed)
  : m_elapsed(elapsed)
{
  validate();
}

auto TickDuration::str() const -> std::string
{
  return std::to_string(m_elapsed);
}

bool TickDuration::operator==(const TickDuration &rhs) const
{
  const auto diff = std::abs(std::abs(m_elapsed) - std::abs(rhs.m_elapsed));
  return diff < TickDuration::TOLERANCE;
}

bool TickDuration::operator<(const TickDuration &rhs) const
{
  return m_elapsed < rhs.m_elapsed;
}

bool TickDuration::operator<=(const TickDuration &rhs) const
{
  return m_elapsed <= rhs.m_elapsed;
}

bool TickDuration::operator>(const TickDuration &rhs) const
{
  return m_elapsed > rhs.m_elapsed;
}

bool TickDuration::operator>=(const TickDuration &rhs) const
{
  return m_elapsed >= rhs.m_elapsed;
}

auto TickDuration::toSeconds() const -> float
{
  constexpr auto SECONDS_IN_TICK = 0.1f;
  return m_elapsed * SECONDS_IN_TICK;
}

void TickDuration::validate()
{
  if (m_elapsed < 0.0f)
  {
    throw std::invalid_argument("Tick duration cannot be negative, got: "
                                + std::to_string(m_elapsed));
  }
}

} // namespace bsgo
