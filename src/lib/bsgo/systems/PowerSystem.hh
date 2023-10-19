
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class PowerSystem : public AbstractSystem
{
  public:
  PowerSystem();
  ~PowerSystem() override = default;

  void updateEntity(Entity &entity,
                    const Coordinator &coordinator,
                    const float elapsedSeconds) override;
};

} // namespace bsgo
