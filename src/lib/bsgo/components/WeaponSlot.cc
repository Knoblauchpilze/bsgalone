
#include "WeaponSlot.hh"

namespace bsgo {

WeaponSlot::WeaponSlot(const Weapon &weapon)
  : SlotComponent("weapon",
                  SlotComponentData{.powerCost  = weapon.powerCost,
                                    .range      = weapon.range,
                                    .reloadTime = weapon.reloadTime})
  , m_minDamage(weapon.minDamage)
  , m_maxDamage(weapon.maxDamage)
{}

auto WeaponSlot::minDamage() const -> float
{
  return m_minDamage;
}

auto WeaponSlot::maxDamage() const -> float
{
  return m_maxDamage;
}

bool WeaponSlot::active() const
{
  return m_active;
}

void WeaponSlot::toggle()
{
  m_active = !m_active;
}

} // namespace bsgo
