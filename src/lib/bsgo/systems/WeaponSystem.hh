
#pragma once

#include "AbstractSystem.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgo {

class WeaponSystem : public AbstractSystem
{
  public:
  WeaponSystem();
  ~WeaponSystem() override = default;

  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const float elapsedSeconds) const override;

  private:
  void updateWeapon(const Entity &ent,
                    const WeaponSlotComponentShPtr &weapon,
                    const std::optional<Entity> &target,
                    const float elapsedSeconds) const;

  void fireWeaponForEntity(Entity &ent,
                           WeaponSlotComponent &weapon,
                           Entity &target,
                           Coordinator &coordinator) const;
  auto updateDamageWithAbilities(Entity &ent, const float damage) const -> float;

  void createBulletDirectedTowards(const Entity &ent,
                                   const Eigen::Vector3f &weaponOffset,
                                   const float damage,
                                   const Entity &target,
                                   Coordinator &coordinator) const;
};

} // namespace bsgo
