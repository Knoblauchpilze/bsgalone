
#include "GameEventType.hh"

namespace bsgalone::server {

auto str(const GameEventType &type) -> std::string
{
  switch (type)
  {
    case GameEventType::PLAYER_LOGIN:
      return "player_login";
    case GameEventType::PLAYER_LOGOUT:
      return "player_logout";
    case GameEventType::PLAYER_SIGNUP:
      return "player_signup";
    case GameEventType::PLAYER_UNDOCK:
      return "player_undock";
    default:
      return "unknown";
  }
}

auto allGameEventTypesAsSet() -> std::unordered_set<GameEventType>
{
  return std::unordered_set<GameEventType>{
    GameEventType::PLAYER_LOGIN,
    GameEventType::PLAYER_LOGOUT,
    GameEventType::PLAYER_SIGNUP,
    GameEventType::PLAYER_UNDOCK,
  };
}

} // namespace bsgalone::server
