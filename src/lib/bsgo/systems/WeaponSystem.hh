
#pragma once

#include "AbstractSystem.hh"

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
  void fireWeaponsForEntity(Entity &ent, Entity &target) const;
  void fireWeaponForEntity(Entity &ent, WeaponSlotComponent &weapon, Entity &target) const;

  auto updateDamageWithAbilities(Entity &ent, const float damage) const -> float;
};

} // namespace bsgo
