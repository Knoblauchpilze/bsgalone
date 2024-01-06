
#include "HealthComponent.hh"

namespace bsgo {

HealthComponent::HealthComponent(const float current, const float max, const float regen)
  : RegenerativeComponent(ComponentType::HEALTH, 0.0f, current, max, regen)
{}

bool HealthComponent::isAlive() const noexcept
{
  return value() >= 0.0f;
}

void HealthComponent::damage(const float damage)
{
  updateValue(-damage);
}

} // namespace bsgo
