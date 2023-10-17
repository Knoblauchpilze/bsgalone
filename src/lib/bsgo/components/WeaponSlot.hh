
#pragma once

#include "SlotComponent.hh"
#include "WeaponRepository.hh"

namespace bsgo {

class WeaponSlot : public SlotComponent
{
  public:
  WeaponSlot(const Weapon &weapon);
  ~WeaponSlot() override = default;

  auto minDamage() const -> float;
  auto maxDamage() const -> float;
  bool active() const;

  void toggle();

  private:
  float m_minDamage;
  float m_maxDamage;
  bool m_active{false};
};

using WeaponSlotShPtr = std::shared_ptr<WeaponSlot>;

} // namespace bsgo
