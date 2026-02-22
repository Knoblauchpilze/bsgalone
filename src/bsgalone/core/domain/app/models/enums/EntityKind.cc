
#include "EntityKind.hh"
#include <stdexcept>

namespace bsgalone::core {

auto str(const EntityKind &kind) noexcept -> std::string
{
  switch (kind)
  {
    case EntityKind::NONE:
      return "none";
    case EntityKind::SHIP:
      return "ship";
    case EntityKind::ASTEROID:
      return "asteroid";
    case EntityKind::OUTPOST:
      return "outpost";
    case EntityKind::PLAYER:
      return "player";
    case EntityKind::BULLET:
      return "bullet";
    default:
      return "unknown(" + std::to_string(static_cast<int>(kind)) + ")";
  }
}

auto toDbEntityKind(const EntityKind &kind) -> std::string
{
  return str(kind);
}

auto fromDbEntityKind(const std::string &dbKind) -> EntityKind
{
  if ("asteroid" == dbKind)
  {
    return EntityKind::ASTEROID;
  }

  throw std::invalid_argument("Unknown entity kind \"" + dbKind + "\"");
}

} // namespace bsgalone::core
