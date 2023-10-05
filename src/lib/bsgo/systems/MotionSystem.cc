
#include "MotionSystem.hh"

namespace bsgo {

MotionSystem::MotionSystem()
  : utils::CoreObject("motion")
{
  setService("system");
}

void MotionSystem::update(const float /*elapsedSeconds*/) {}

} // namespace bsgo
