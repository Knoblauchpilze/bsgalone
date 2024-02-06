
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class RemovalSystem : public AbstractSystem
{
  public:
  RemovalSystem();
  ~RemovalSystem() override = default;

  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const float elapsedSeconds) const override;
};

} // namespace bsgo
