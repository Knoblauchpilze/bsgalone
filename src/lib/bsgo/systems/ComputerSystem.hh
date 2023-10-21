
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class ComputerSystem : public AbstractSystem
{
  public:
  ComputerSystem();
  ~ComputerSystem() override = default;

  void updateEntity(Entity &entity,
                    const Coordinator &coordinator,
                    const float elapsedSeconds) override;
};

} // namespace bsgo
