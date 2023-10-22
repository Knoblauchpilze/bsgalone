
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class ComputerSystem : public AbstractSystem
{
  public:
  ComputerSystem();
  ~ComputerSystem() override = default;

  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const float elapsedSeconds) const override;
};

} // namespace bsgo
