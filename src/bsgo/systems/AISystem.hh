
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class AISystem : public AbstractSystem
{
  public:
  AISystem();
  ~AISystem() override = default;

  protected:
  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const chrono::TickData &data) const override;
};

} // namespace bsgo
