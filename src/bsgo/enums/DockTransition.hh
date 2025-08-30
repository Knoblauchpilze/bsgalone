
#pragma once

#include <string>

namespace bsgo {

enum class DockTransition
{
  DOCK,
  UNDOCK,
  BACK_TO_OUTPOST
};

auto str(const DockTransition transition) -> std::string;

} // namespace bsgo
