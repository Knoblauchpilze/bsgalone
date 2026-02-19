
#pragma once

#include <string>

namespace bsgalone::core {

enum class DockTransition
{
  DOCK,
  UNDOCK,
  BACK_TO_OUTPOST
};

auto str(const DockTransition transition) -> std::string;

} // namespace bsgalone::core
