
#include "GameEventType.hh"

namespace bsgalone::core {

auto str(const GameEventType &type) -> std::string
{
  switch (type)
  {
    case GameEventType::PLAYER_LOGIN:
      return "player_login";
    default:
      return "unknown";
  }
}

auto allGameEventTypesAsSet() -> std::unordered_set<GameEventType>
{
  return std::unordered_set<GameEventType>{
    GameEventType::PLAYER_LOGIN,
  };
}

} // namespace bsgalone::core
