
#pragma once

#include <string>

namespace bsgo {

enum class LoadingTransition
{
  JUMP,
  LOGIN,
  UNDOCK,
};

auto str(const LoadingTransition transition) -> std::string;

} // namespace bsgo
