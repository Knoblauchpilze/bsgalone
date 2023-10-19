
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class MotionSystem : public AbstractSystem
{
  public:
  MotionSystem();
  ~MotionSystem() override = default;

  void updateEntity(Entity &entity,
                    const Coordinator &coordinator,
                    const float elapsedSeconds) override;
};

} // namespace bsgo
