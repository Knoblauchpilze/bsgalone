
#pragma once

#include "TickDuration.hh"
#include <string>

namespace bsgo {

class Tick
{
  public:
  Tick() = default;
  Tick(const float in);
  Tick(const int count, const float frac);
  ~Tick() = default;

  auto count() const -> int;
  auto frac() const -> float;
  auto elapsed() const -> float;

  auto str() const -> std::string;

  auto operator+=(const TickDuration &duration) -> Tick &;

  auto operator<(const Tick &rhs) const -> bool;
  auto operator<=(const Tick &rhs) const -> bool;
  auto operator>(const Tick &rhs) const -> bool;
  auto operator>=(const Tick &rhs) const -> bool;

  auto serialize(std::ostream &out) const -> std::ostream &;
  bool deserialize(std::istream &in);

  static auto fromInt(const int duration) -> Tick;

  /// @brief - Computes lhs - rhs and returns the result as a `TickDuration`.
  /// In case `lhs < rhs`, the return value is equivalent to `TickDuration()`.
  /// This means that there's no negative duration returned by this method.
  /// @param lhs - the tick to subtract from
  /// @param rhs - the tick to subtract
  /// @return - the result of `lhs - rhs` or `TickDuration()` in case `lhs < rhs`
  static auto safeSubtract(const Tick &lhs, const Tick &rhs) -> TickDuration;

  private:
  int m_count{0};
  float m_frac{0.0f};

  void validate();
};

auto operator<<(std::ostream &out, const Tick &tick) -> std::ostream &;
auto operator>>(std::istream &in, Tick &tick) -> std::istream &;

} // namespace bsgo
