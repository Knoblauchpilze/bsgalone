
#include "DamageComponent.hh"

namespace bsgalone::core {

DamageComponent::DamageComponent(const float damage)
  : AbstractComponent(ComponentType::DAMAGE)
  , m_damage(damage)
{}

auto DamageComponent::damage() const noexcept -> float
{
  return m_damage;
}

void DamageComponent::update(const chrono::TickData & /*data*/) {}

} // namespace bsgalone::core
