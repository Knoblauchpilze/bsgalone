
#include "Tick.hh"
#include <cmath>
#include <stdexcept>

namespace chrono {
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

void Tick::validate()
{
  if (m_count < 0 || m_frac < 0.0f || m_frac >= 1.0f)
  {
    throw std::invalid_argument("Invalid tick " + std::to_string(m_count) + "/"
                                + std::to_string(m_frac));
  }
}

} // namespace chrono
