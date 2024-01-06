
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class StatusSystem : public AbstractSystem
{
  public:
  StatusSystem();
  ~StatusSystem() override = default;

  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const float elapsedSeconds) const override;

  private:
  void handleAppearingState(Entity &entity, StatusComponent &statusComp) const;
  void handleThreatState(Entity &entity, StatusComponent &statusComp) const;
  void handleJustChangedState(Entity &entity, StatusComponent &statusComp) const;
  void handleJumpState(Entity &entity, StatusComponent &statusComp) const;
};

} // namespace bsgo
