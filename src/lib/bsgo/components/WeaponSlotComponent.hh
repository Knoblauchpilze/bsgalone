
#pragma once

#include "PlayerWeaponRepository.hh"
#include "SlotComponent.hh"

namespace bsgo {

class WeaponSlotComponent : public SlotComponent
{
  public:
  WeaponSlotComponent(const PlayerWeapon &weapon);
  ~WeaponSlotComponent() override = default;

  auto minDamage() const -> float;
  auto maxDamage() const -> float;
  bool active() const;

  void toggle();

  auto generateDamage() const -> float;

  private:
  float m_minDamage;
  float m_maxDamage;
  bool m_active{false};
};

using WeaponSlotComponentShPtr = std::shared_ptr<WeaponSlotComponent>;

} // namespace bsgo
