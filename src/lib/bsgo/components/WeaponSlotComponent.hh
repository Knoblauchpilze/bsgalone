
#pragma once

#include "SlotComponent.hh"
#include "WeaponRepository.hh"

namespace bsgo {

class WeaponSlotComponent : public SlotComponent
{
  public:
  WeaponSlotComponent(const Weapon &weapon);
  ~WeaponSlotComponent() override = default;

  auto minDamage() const -> float;
  auto maxDamage() const -> float;
  bool active() const;

  void toggle();

  private:
  float m_minDamage;
  float m_maxDamage;
  bool m_active{false};
};

using WeaponSlotComponentShPtr = std::shared_ptr<WeaponSlotComponent>;

} // namespace bsgo
