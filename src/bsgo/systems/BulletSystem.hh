
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class BulletSystem : public AbstractSystem
{
  public:
  BulletSystem();
  ~BulletSystem() override = default;

  protected:
  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const chrono::TickData &data) const override;

  private:
  bool isTargetNotExistent(const Entity &entity) const;
  void accelerateTowardsTarget(Entity &entity, const Entity &target) const;
  void damageOnImpact(Entity &entity, Entity &target) const;
};

} // namespace bsgo
