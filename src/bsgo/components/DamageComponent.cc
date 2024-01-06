
#include "DamageComponent.hh"

namespace bsgo {

DamageComponent::DamageComponent(const float damage)
  : AbstractComponent(ComponentType::DAMAGE)
  , m_damage(damage)
{}

auto DamageComponent::damage() const noexcept -> float
{
  return m_damage;
}

void DamageComponent::update(const float /*elapsedSeconds*/) {}

} // namespace bsgo
