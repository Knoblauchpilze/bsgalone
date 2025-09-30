
#pragma once

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

  auto str() const -> std::string;

  auto operator+=(const Tick &rhs) -> Tick &;

  auto serialize(std::ostream &out) const -> std::ostream &;
  bool deserialize(std::istream &in);

  static auto fromInt(const int duration) -> Tick;

  private:
  int m_count{0};
  float m_frac{0.0f};

  void validate();
};

auto operator<<(std::ostream &out, const Tick &tick) -> std::ostream &;
auto operator>>(std::istream &in, Tick &tick) -> std::istream &;

} // namespace bsgo
