
#pragma once

#include <string>
#include <unordered_set>

namespace bsgalone::core {

enum class GameEventType
{
  PLAYER_LOGIN,
  PLAYER_SIGNUP
};

auto str(const GameEventType &type) -> std::string;

auto allGameEventTypesAsSet() -> std::unordered_set<GameEventType>;

} // namespace bsgalone::core
