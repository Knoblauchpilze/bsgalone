
#include "Faction.hh"

namespace bsgo {

auto str(const Faction faction) -> std::string
{
  switch (faction)
  {
    case Faction::COLONIAL:
      return "colonial";
    case Faction::CYLON:
      return "cylon";
    default:
      return "unknown";
  }
}

} // namespace bsgo
