
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class HealthSystem : public AbstractSystem
{
  public:
  HealthSystem();
  ~HealthSystem() override = default;

  protected:
  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const chrono::TickData &data) const override;

  private:
  bool tryMarkForDelettion(Entity &entity) const;
  bool canRegenerateHealth(Entity &entity) const;
};

} // namespace bsgo
