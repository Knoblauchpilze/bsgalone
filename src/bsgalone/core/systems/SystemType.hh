
#pragma once

#include <string>

namespace bsgalone::core {

enum class SystemType
{
  AI,
  BULLET,
  COMPUTER,
  EFFECT,
  HEALTH,
  LOOT,
  MOTION,
  NETWORK,
  REMOVAL,
  POWER,
  STATUS,
  TARGET,
  WEAPON
};

auto str(const SystemType &type) -> std::string;

} // namespace bsgalone::core
