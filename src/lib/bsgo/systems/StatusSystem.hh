
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class StatusSystem : public AbstractSystem
{
  public:
  StatusSystem();
  ~StatusSystem() override = default;

  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const float elapsedSeconds) const override;
};

} // namespace bsgo
