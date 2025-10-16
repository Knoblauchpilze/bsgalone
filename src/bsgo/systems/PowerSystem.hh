
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class PowerSystem : public AbstractSystem
{
  public:
  PowerSystem();
  ~PowerSystem() override = default;

  protected:
  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const chrono::TickData &data) const override;

  private:
  bool canRegeneratePower(Entity &entity) const;
};

} // namespace bsgo
