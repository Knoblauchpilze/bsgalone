
#pragma once

#include <string>

namespace bsgo {

enum class LoadingTransition
{
  ACTIVE_SHIP_CHANGED,
  DOCK,
  JUMP,
  LOGIN,
  PURCHASE,
  UNDOCK,
};

auto str(const LoadingTransition transition) -> std::string;

} // namespace bsgo
