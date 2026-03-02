
#pragma once

#include <string>

namespace bsgalone::core {

enum class ComponentType
{
  AI,
  COMPUTER_SLOT,
  DAMAGE,
  DB,
  DB_SYNC,
  FACTION,
  HEALTH,
  KIND,
  LOOT,
  NAME,
  NETWORK_SYNC,
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

} // namespace bsgalone::core
