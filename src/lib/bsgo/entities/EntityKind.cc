
#include "EntityKind.hh"

namespace bsgo {

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

} // namespace bsgo
