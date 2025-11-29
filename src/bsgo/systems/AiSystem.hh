
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class AiSystem : public AbstractSystem
{
  public:
  AiSystem();
  ~AiSystem() override = default;

  protected:
  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const chrono::TickData &data) const override;

  void triggerAiBehaviorSync(Entity &entity) const;
};

} // namespace bsgo
