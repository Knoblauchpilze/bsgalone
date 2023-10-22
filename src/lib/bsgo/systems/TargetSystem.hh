
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class TargetSystem : public AbstractSystem
{
  public:
  TargetSystem();
  ~TargetSystem() override = default;

  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const float elapsedSeconds) const override;

  private:
  void clearTargetIfDead(TargetComponent &targetComp, const Coordinator &coordinator) const;
};

} // namespace bsgo
