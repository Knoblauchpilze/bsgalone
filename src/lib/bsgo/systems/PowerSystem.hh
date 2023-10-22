
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class PowerSystem : public AbstractSystem
{
  public:
  PowerSystem();
  ~PowerSystem() override = default;

  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const float elapsedSeconds) const override;
};

} // namespace bsgo
