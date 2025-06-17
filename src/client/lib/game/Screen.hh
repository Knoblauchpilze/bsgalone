
#pragma once

#include <string>

namespace pge {

enum class Screen
{
  LOGIN,
  LOADING,
  GAME,
  MAP,
  OUTPOST,
  EXIT
};

auto str(const Screen s) noexcept -> std::string;

} // namespace pge
