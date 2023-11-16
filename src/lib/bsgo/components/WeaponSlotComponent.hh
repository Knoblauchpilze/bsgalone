
#pragma once

#include "PlayerWeaponRepository.hh"
#include "SlotComponent.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgo {

class WeaponSlotComponent : public SlotComponent
{
  public:
  WeaponSlotComponent(const PlayerWeapon &weapon, const Eigen::Vector3f &position);
  ~WeaponSlotComponent() override = default;

  auto minDamage() const -> float;
  auto maxDamage() const -> float;
  bool active() const;
  auto position() const -> Eigen::Vector3f;

  void toggle();

  auto generateDamage() const -> float;

  private:
  float m_minDamage;
  float m_maxDamage;
  bool m_active{false};
  Eigen::Vector3f m_position;
};

using WeaponSlotComponentShPtr = std::shared_ptr<WeaponSlotComponent>;

} // namespace bsgo
