
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

  private:
  void triggerAiBehaviorSync(Entity &entity, Coordinator &coordinator) const;
  void trySendAiBehaviorSyncMessage(const Entity &entity) const;
  void trySendTargetMessage(const Entity &entity, Coordinator &coordinator) const;
};

} // namespace bsgo
