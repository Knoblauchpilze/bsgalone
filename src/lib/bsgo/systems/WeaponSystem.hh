
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class WeaponSystem : public AbstractSystem
{
  public:
  WeaponSystem();
  ~WeaponSystem() override = default;

  void updateEntity(Entity &entity,
                    const Coordinator &coordinator,
                    const float elapsedSeconds) override;

  private:
  void fireWeaponsForEntity(Entity &ent, Entity &target);
  void fireWeaponForEntity(Entity &ent, WeaponSlotComponent &weapon, Entity &target);
};

} // namespace bsgo
