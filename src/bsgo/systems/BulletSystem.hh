
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class BulletSystem : public AbstractSystem
{
  public:
  BulletSystem();
  ~BulletSystem() override = default;

  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const float elapsedSeconds) const override;

  private:
  bool isTargetNotExistent(const Entity &entity) const;
  bool isTargetInvalid(const Entity &target) const;
  void accelerateTowardsTarget(Entity &entity, const Entity &target) const;
  void damageOnImpact(Entity &entity, Entity &target) const;
};

} // namespace bsgo
