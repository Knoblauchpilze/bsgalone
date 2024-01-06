
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class OwnerSystem : public AbstractSystem
{
  public:
  OwnerSystem();
  ~OwnerSystem() override = default;

  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const float elapsedSeconds) const override;
};

} // namespace bsgo
