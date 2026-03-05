
#include "SystemType.hh"

namespace bsgo {

auto str(const SystemType &type) -> std::string
{
  switch (type)
  {
    case SystemType::AI:
      return "ai";
    case SystemType::BULLET:
      return "bullet";
    case SystemType::COMPUTER:
      return "computer";
    case SystemType::EFFECT:
      return "effect";
    case SystemType::HEALTH:
      return "health";
    case SystemType::LOOT:
      return "loot";
    case SystemType::MOTION:
      return "motion";
    case SystemType::NETWORK:
      return "network";
    case SystemType::REMOVAL:
      return "removal";
    case SystemType::POWER:
      return "power";
    case SystemType::STATUS:
      return "status";
    case SystemType::TARGET:
      return "target";
    case SystemType::WEAPON:
      return "weapon";
    default:
      return "unknown";
  }
}

} // namespace bsgo
