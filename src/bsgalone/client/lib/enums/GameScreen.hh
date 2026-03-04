
#pragma once

#include <string>

namespace bsgalone::client {

enum class GameScreen
{
  LOGIN,
  LOADING,
  GAME,
  EXIT
};

auto str(const GameScreen s) noexcept -> std::string;

} // namespace bsgalone::client
