
#include "MotionSystem.hh"

namespace bsgo {

MotionSystem::MotionSystem()
  : utils::CoreObject("motion")
{
  setService("system");
}

} // namespace bsgo
