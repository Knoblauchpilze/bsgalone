
#pragma once

#include <string>

namespace bsgo {

enum class LoadingTransition
{
  DOCK,
  JUMP,
  LOGIN,
  PURCHASE,
  UNDOCK,
};

auto str(const LoadingTransition transition) -> std::string;

} // namespace bsgo
