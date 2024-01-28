
#include "ComponentType.hh"

namespace bsgo {

auto str(const ComponentType &type) -> std::string
{
  switch (type)
  {
    case ComponentType::AI:
      return "ai";
    case ComponentType::COMPUTER_SLOT:
      return "computer_slot";
    case ComponentType::DAMAGE:
      return "damage";
    case ComponentType::DB:
      return "db";
    case ComponentType::FACTION:
      return "faction";
    case ComponentType::HEALTH:
      return "health";
    case ComponentType::KIND:
      return "kind";
    case ComponentType::LOOT:
      return "loot";
    case ComponentType::NAME:
      return "name";
    case ComponentType::NETWORK:
      return "network";
    case ComponentType::OWNER:
      return "owner";
    case ComponentType::POWER:
      return "power";
    case ComponentType::REMOVAL:
      return "removal";
    case ComponentType::RESOURCE:
      return "resource";
    case ComponentType::SCANNED:
      return "scanned";
    case ComponentType::SHIP_CLASS:
      return "ship_class";
    case ComponentType::STATUS:
      return "status";
    case ComponentType::TARGET:
      return "target";
    case ComponentType::TRANSFORM:
      return "transform";
    case ComponentType::VELOCITY:
      return "velocity";
    case ComponentType::WEAPON_EFFECT:
      return "weapon_effect";
    case ComponentType::WEAPON_SLOT:
      return "weapon_slot";
    default:
      return "unknown";
  }
}

} // namespace bsgo
