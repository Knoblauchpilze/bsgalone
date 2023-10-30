
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
};

} // namespace bsgo
