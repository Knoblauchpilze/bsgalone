
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class MotionSystem : public AbstractSystem
{
  public:
  MotionSystem();
  ~MotionSystem() override = default;

  protected:
  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const chrono::TickData &data) const override;
};

} // namespace bsgo
