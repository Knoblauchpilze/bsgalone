
#pragma once

#include "ISystem.hh"
#include "Uuid.hh"
#include <core_utils/CoreObject.hh>

namespace bsgo {

class MotionSystem : public utils::CoreObject, public ISystem
{
  public:
  MotionSystem();
  ~MotionSystem() override = default;

  void update(const Components &components, const float elapsedSeconds) override;
};

} // namespace bsgo
