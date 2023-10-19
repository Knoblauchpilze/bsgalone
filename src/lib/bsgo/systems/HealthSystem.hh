
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class HealthSystem : public AbstractSystem
{
  public:
  HealthSystem();
  ~HealthSystem() override = default;

  void updateEntity(Entity &entity,
                    const Coordinator &coordinator,
                    const float elapsedSeconds) override;
};

} // namespace bsgo
