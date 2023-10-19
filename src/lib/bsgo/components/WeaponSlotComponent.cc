
#include "WeaponSlotComponent.hh"

namespace bsgo {

WeaponSlotComponent::WeaponSlotComponent(const Weapon &weapon)
  : SlotComponent("weapon",
                  SlotComponentData{.powerCost  = weapon.powerCost,
                                    .range      = weapon.range,
                                    .reloadTime = weapon.reloadTime})
  , m_minDamage(weapon.minDamage)
  , m_maxDamage(weapon.maxDamage)
{}

auto WeaponSlotComponent::minDamage() const -> float
{
  return m_minDamage;
}

auto WeaponSlotComponent::maxDamage() const -> float
{
  return m_maxDamage;
}

bool WeaponSlotComponent::active() const
{
  return m_active;
}

void WeaponSlotComponent::toggle()
{
  m_active = !m_active;
}

auto WeaponSlotComponent::generateDamage() const -> float
{
  return (m_minDamage + m_maxDamage) / 2.0f;
}

} // namespace bsgo
