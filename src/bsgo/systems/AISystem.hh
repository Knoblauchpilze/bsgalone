
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class AISystem : public AbstractSystem
{
  public:
  AISystem();
  ~AISystem() override = default;

  void updateEntity(Entity &entity, Coordinator &coordinator, const TickData &data) const override;
};

} // namespace bsgo
