
#pragma once

#include "RegenerativeComponent.hh"

namespace bsgalone::core {

class HealthComponent : public RegenerativeComponent
{
  public:
  HealthComponent(const float current, const float max, const float regen);
  ~HealthComponent() override = default;

  bool isAlive() const noexcept;
  void damage(const float damage);
};

using HealthComponentShPtr = std::shared_ptr<HealthComponent>;

} // namespace bsgalone::core
