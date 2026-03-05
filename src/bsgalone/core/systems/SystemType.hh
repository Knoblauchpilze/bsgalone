
#pragma once

#include <string>

namespace bsgo {

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

} // namespace bsgo
