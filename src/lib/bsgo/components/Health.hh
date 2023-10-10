
#pragma once

#include <memory>

namespace bsgo {

class Health
{
  public:
  Health() = default;
  Health(const float current, const float max);
  ~Health() = default;

  auto health() const -> float;
  auto max() const -> float;

  private:
  float m_current{1.0f};
  float m_max{1.0f};

  void validate();
};

using HealthShPtr = std::shared_ptr<Health>;

} // namespace bsgo
