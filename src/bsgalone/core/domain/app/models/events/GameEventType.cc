
#include "GameEventType.hh"

namespace bsgalone::core {

auto str(const GameEventType &type) -> std::string
{
  switch (type)
  {
    case GameEventType::ENTITY_DIED:
      return "entity_died";
    case GameEventType::JUMP_COMPLETED:
      return "jump_completed";
    default:
      return "unknown";
  }
}

auto allGameEventTypesAsSet() -> std::unordered_set<GameEventType>
{
  return std::unordered_set<GameEventType>{
    GameEventType::ENTITY_DIED,
    GameEventType::JUMP_COMPLETED,
  };
}

} // namespace bsgalone::core
