
#include "Tick.hh"
#include "SerializationUtils.hh"
#include <cmath>
#include <stdexcept>

namespace bsgo {
namespace {
auto cast(const float in) -> std::tuple<int, float>
{
  double intPart{};
  const auto fracPart = std::modf(static_cast<double>(in), &intPart);

  return std::make_pair(static_cast<int>(intPart), static_cast<float>(fracPart));
}
} // namespace

Tick::Tick(const float in)
{
  std::tie(m_count, m_frac) = cast(in);
  validate();
}

Tick::Tick(const int count, const float frac)
  : m_count(count)
  , m_frac(frac)
{
  validate();
}

auto Tick::count() const -> int
{
  return m_count;
}

auto Tick::frac() const -> float
{
  return m_frac;
}

auto Tick::str() const -> std::string
{
  return std::to_string(m_count) + "[" + std::to_string(m_frac) + "]";
}

auto Tick::operator+=(const Tick &rhs) -> Tick &
{
  m_count += rhs.m_count;
  const auto frac = m_frac + rhs.m_frac;

  int carryOver{};
  std::tie(carryOver, m_frac) = cast(frac);
  m_count += carryOver;
  validate();

  return *this;
}

auto Tick::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_count);
  core::serialize(out, m_frac);

  return out;
}

bool Tick::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_count);
  ok &= core::deserialize(in, m_frac);

  return ok;
}

auto Tick::fromInt(const int duration) -> Tick
{
  return Tick(duration, 0.0f);
}

void Tick::validate()
{
  if (m_count < 0 || m_frac < 0.0f || m_frac >= 1.0f)
  {
    throw std::invalid_argument("Invalid tick " + std::to_string(m_count) + "/"
                                + std::to_string(m_frac));
  }
}

auto operator<<(std::ostream &out, const Tick &tick) -> std::ostream &
{
  tick.serialize(out);
  return out;
}

auto operator>>(std::istream &in, Tick &tick) -> std::istream &
{
  tick.deserialize(in);
  return in;
}

} // namespace bsgo
