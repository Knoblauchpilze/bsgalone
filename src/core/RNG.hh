
#pragma once

#include "CoreObject.hh"
#include <istream>
#include <ostream>
#include <random>

namespace core {

class RNG : public CoreObject
{
  public:
  RNG(int seed = 0);

  auto rndInt(const int minIncluded, const int maxIncluded) noexcept -> int;
  auto rndFloat(const float minIncluded, const float maxIncluded) noexcept -> float;
  auto rndAngle(const float minIncluded = 0.0f, const float maxIncluded = 6.283185307f) noexcept
    -> float;

  auto operator<<(std::ostream &out) const -> std::ostream &;
  auto operator>>(std::istream &in) -> std::istream &;

  static auto createFromRandomSeed() noexcept -> RNG;

  private:
  std::mt19937 m_rng{};
};

} // namespace core

auto operator<<(std::ostream &out, const core::RNG &rng) noexcept -> std::ostream &;
auto operator>>(std::istream &in, core::RNG &rng) noexcept -> std::istream &;
