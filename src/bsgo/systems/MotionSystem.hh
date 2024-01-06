
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class MotionSystem : public AbstractSystem
{
  public:
  MotionSystem();
  ~MotionSystem() override = default;

  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const float elapsedSeconds) const override;
};

} // namespace bsgo
