
#pragma once

#include <string>

namespace chrono {

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

  private:
  int m_count{0};
  float m_frac{0.0f};

  void validate();
};

} // namespace chrono
