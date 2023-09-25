
#pragma once

#include <string>

namespace pge {

enum class Screen
{
  LOGIN,
  GAME,
  MAP,
  OUTPOST,
  GAMEOVER,
  EXIT
};

auto str(const Screen s) noexcept -> std::string;

} // namespace pge
