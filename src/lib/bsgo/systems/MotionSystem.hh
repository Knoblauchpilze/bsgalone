
#pragma once

#include "ISystem.hh"

namespace bsgo {

class MotionSystem : public ISystem
{
  public:
  MotionSystem();
  ~MotionSystem() override = default;

  void update(const Components &components, const float elapsedSeconds) override;
};

} // namespace bsgo
