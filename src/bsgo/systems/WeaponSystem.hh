
#pragma once

#include "AbstractSystem.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgo {

class WeaponSystem : public AbstractSystem
{
  public:
  WeaponSystem();
  ~WeaponSystem() override = default;

  protected:
  void updateEntity(Entity &entity, Coordinator &coordinator, const TickData &data) const override;

  private:
  bool canTargetBeFiredOn(const Entity &target) const;

  void updateWeapon(const Entity &ent,
                    const WeaponSlotComponentShPtr &weapon,
                    const std::optional<Entity> &target,
                    const TickData &data) const;

  void fireWeaponForEntity(Entity &ent,
                           WeaponSlotComponent &weapon,
                           Entity &target,
                           Coordinator &coordinator) const;
  auto updateDamageWithAbilities(Entity &ent, const float damage) const -> float;

  void createBulletDirectedTowards(const Entity &ent,
                                   const Eigen::Vector3f &weaponPosition,
                                   const float damage,
                                   const Entity &target,
                                   Coordinator &coordinator) const;
};

} // namespace bsgo
