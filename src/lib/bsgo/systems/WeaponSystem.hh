
#pragma once

#include "Entity.hh"
#include "ISystem.hh"

namespace bsgo {

class WeaponSystem : public ISystem
{
  public:
  WeaponSystem();
  ~WeaponSystem() override = default;

  void update(const Components &components,
              const Coordinator &coordinator,
              const float elapsedSeconds) override;

  private:
  void fireWeaponsForEntity(Entity &ent, Entity &target);
  void fireWeaponForEntity(Entity &ent, WeaponSlotComponent &weapon, Entity &target);
};

} // namespace bsgo
