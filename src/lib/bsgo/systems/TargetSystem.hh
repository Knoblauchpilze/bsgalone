
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class TargetSystem : public AbstractSystem
{
  public:
  TargetSystem();
  ~TargetSystem() override = default;

  void updateEntity(Entity &entity,
                    const Coordinator &coordinator,
                    const float elapsedSeconds) override;

  private:
  void clearTargetIfDead(TargetComponent &targetComp, const Coordinator &coordinator);
};

} // namespace bsgo
