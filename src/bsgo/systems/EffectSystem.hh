
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class EffectSystem : public AbstractSystem
{
  public:
  EffectSystem();
  ~EffectSystem() override = default;

  void updateEntity(Entity &entity, Coordinator &coordinator, const TickData &data) const override;

  private:
  void cleanUpFinishedEffects(const Entity &entity, Coordinator &coordinator) const;
};

} // namespace bsgo
