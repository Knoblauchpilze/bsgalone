
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
    default:
      return "unknown(" + std::to_string(static_cast<int>(kind)) + ")";
  }
}

} // namespace bsgo
