
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class LootSystem : public AbstractSystem
{
  public:
  LootSystem();
  ~LootSystem() override = default;

  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const float elapsedSeconds) const override;
};

} // namespace bsgo
