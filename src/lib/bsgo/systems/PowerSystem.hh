
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

  private:
  bool canRegeneratePower(Entity &entity) const;
};

} // namespace bsgo
