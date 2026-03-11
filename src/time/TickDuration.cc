
#include "TickDuration.hh"
#include "SerializationUtils.hh"
#include <cmath>
#include <stdexcept>

namespace chrono {

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

auto TickDuration::operator+=(const TickDuration &duration) -> TickDuration &
{
  m_elapsed += duration.m_elapsed;
  return *this;
}

auto TickDuration::operator/(const TickDuration &duration) const -> float
{
  return m_elapsed / duration.m_elapsed;
}

auto TickDuration::operator*(const float rhs) const -> float
{
  return rhs * m_elapsed;
}

auto TickDuration::operator-(const TickDuration &rhs) const -> TickDuration
{
  const auto duration = m_elapsed - rhs.m_elapsed;

  if (duration < 0.0f)
  {
    return TickDuration();
  }

  return TickDuration(duration);
}

auto TickDuration::fromInt(const int duration) -> TickDuration
{
  return TickDuration(static_cast<float>(duration));
}

void TickDuration::validate()
{
  if (m_elapsed < 0.0f)
  {
    throw std::invalid_argument("Tick duration cannot be negative, got: "
                                + std::to_string(m_elapsed));
  }
}

auto operator*(const float lhs, const TickDuration &rhs) -> float
{
  return rhs * lhs;
}

auto operator<<(std::ostream &out, const TickDuration &tick) -> std::ostream &
{
  core::serialize(out, tick.m_elapsed);

  return out;
}

bool operator>>(std::istream &in, TickDuration &tick)
{
  bool ok{true};
  ok &= core::deserialize(in, tick.m_elapsed);

  return ok;
}

} // namespace chrono
