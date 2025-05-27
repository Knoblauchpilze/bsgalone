
#include "RNG.hh"

namespace core {

RNG::RNG(int seed)
  : CoreObject("rng")
  , m_rng(seed)
{}

auto RNG::rndInt(const int minIncluded, const int maxIncluded) noexcept -> int
{
  std::uniform_int_distribution<int> dist(minIncluded, maxIncluded);
  return dist(m_rng);
}

auto RNG::rndFloat(const float minIncluded, const float maxIncluded) noexcept -> float
{
  std::uniform_real_distribution<float> dist(minIncluded, maxIncluded);
  return dist(m_rng);
}

auto RNG::rndAngle(const float minIncluded, const float maxIncluded) noexcept -> float
{
  return rndFloat(minIncluded, maxIncluded);
}

auto RNG::operator<<(std::ostream &out) const -> std::ostream &
{
  out << m_rng;
  return out;
}

auto RNG::operator>>(std::istream &in) -> std::istream &
{
  in >> m_rng;
  return in;
}

auto RNG::createFromRandomSeed() noexcept -> RNG
{
  // See: https://diego.assencio.com/?index=6890b8c50169ef45b74db135063c227c.
  std::random_device device;
  return RNG(device());
}

} // namespace core

inline std::ostream &operator<<(std::ostream &out, const core::RNG &rng) noexcept
{
  rng << out;
  return out;
}

inline std::istream &operator>>(std::istream &in, core::RNG &rng) noexcept
{
  rng >> in;
  return in;
}
