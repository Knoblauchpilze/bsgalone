
#pragma once

#include "AbstractSystem.hh"

namespace bsgalone::core {

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
  void triggerAiBehaviorSync(Entity &entity) const;
  void trySendAiBehaviorSyncMessage(const Entity &entity) const;
};

} // namespace bsgalone::core
