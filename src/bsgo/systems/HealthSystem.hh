
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class HealthSystem : public AbstractSystem
{
  public:
  HealthSystem();
  ~HealthSystem() override = default;

  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const float elapsedSeconds) const override;

  private:
  bool tryMarkForDelettion(Entity &entity) const;
  bool canRegenerateHealth(Entity &entity) const;
};

} // namespace bsgo
