
#pragma once

#include "SlotComponent.hh"
#include "WeaponRepository.hh"

namespace bsgo {

class WeaponSlot : public SlotComponent
{
  public:
  WeaponSlot(const Weapon &weapon);
  ~WeaponSlot() override = default;

  private:
  float m_minDamage;
  float m_maxDamage;
};

using WeaponSlotShPtr = std::shared_ptr<WeaponSlot>;

} // namespace bsgo
