
#pragma once

#include <string>

namespace bsgo {

enum class ComponentType
{
  AI,
  COMPUTER_SLOT,
  DAMAGE,
  DB,
  FACTION,
  HEALTH,
  KIND,
  LOOT,
  NAME,
  NETWORK,
  OWNER,
  POWER,
  REMOVAL,
  RESOURCE,
  SCANNED,
  SHIP_CLASS,
  STATUS,
  TARGET,
  TRANSFORM,
  VELOCITY,
  WEAPON_EFFECT,
  WEAPON_SLOT
};

auto str(const ComponentType &type) -> std::string;

} // namespace bsgo
