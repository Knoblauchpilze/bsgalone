
#pragma once

#include <string>

namespace bsgo {

enum class LoadingTransition
{
  JUMP,
  LOGIN,
  UNDOCK,
  PURCHASE,
};

auto str(const LoadingTransition transition) -> std::string;

} // namespace bsgo
