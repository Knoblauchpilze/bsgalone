
#pragma once

#include <string>

namespace bsgalone::core {

enum class LoadingTransition
{
  ACTIVE_SHIP_CHANGED,
  DOCK,
  EQUIP,
  JUMP,
  LOGIN,
  PURCHASE,
  UNDOCK,
};

auto str(const LoadingTransition transition) -> std::string;

} // namespace bsgalone::core
