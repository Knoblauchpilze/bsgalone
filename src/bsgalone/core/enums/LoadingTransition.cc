
#include "LoadingTransition.hh"

namespace bsgo {

auto str(const LoadingTransition transition) -> std::string
{
  switch (transition)
  {
    case LoadingTransition::ACTIVE_SHIP_CHANGED:
      return "active_ship_changed";
    case LoadingTransition::DOCK:
      return "dock";
    case LoadingTransition::EQUIP:
      return "equip";
    case LoadingTransition::JUMP:
      return "jump";
    case LoadingTransition::LOGIN:
      return "login";
    case LoadingTransition::PURCHASE:
      return "purchase";
    case LoadingTransition::UNDOCK:
      return "undock";
    default:
      return "unknown";
  }
}

} // namespace bsgo
