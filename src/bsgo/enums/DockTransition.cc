
#include "DockTransition.hh"

namespace bsgo {

auto str(const DockTransition transition) -> std::string
{
  switch (transition)
  {
    case DockTransition::DOCK:
      return "dock";
    case DockTransition::UNDOCK:
      return "undock";
    case DockTransition::BACK_TO_OUTPOST:
      return "back_to_outpost";
    default:
      return "unknown";
  }
}

} // namespace bsgo
