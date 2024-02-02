
#include "WeaponSlotComponent.hh"

namespace bsgo {

WeaponSlotComponent::WeaponSlotComponent(const PlayerWeapon &weapon, const Eigen::Vector3f &position)
  : SlotComponent(ComponentType::WEAPON_SLOT,
                  SlotComponentData{.dbId       = weapon.id,
                                    .offensive  = true,
                                    .powerCost  = weapon.powerCost,
                                    .range      = {weapon.range},
                                    .reloadTime = weapon.reloadTime})
  , m_minDamage(weapon.minDamage)
  , m_maxDamage(weapon.maxDamage)
  , m_position(position)
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

auto WeaponSlotComponent::position() const -> Eigen::Vector3f
{
  return m_position;
}

void WeaponSlotComponent::setActive(const bool active)
{
  m_active = active;
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
