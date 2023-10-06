
#include "MotionSystem.hh"

namespace bsgo {

MotionSystem::MotionSystem()
  : utils::CoreObject("motion")
{
  setService("system");
}

void MotionSystem::update(const Components & /*components*/, const float /*elapsedSeconds*/) {}

} // namespace bsgo
