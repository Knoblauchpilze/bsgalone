
#include "Faction.hh"
#include <stdexcept>

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

auto toDbFaction(const Faction faction) -> std::string
{
  return str(faction);
}

auto fromDbFaction(const std::string &dbFaction) -> Faction
{
  if ("colonial" == dbFaction)
  {
    return Faction::COLONIAL;
  }
  if ("cylon" == dbFaction)
  {
    return Faction::CYLON;
  }

  throw std::invalid_argument("Unknown faction \"" + dbFaction + "\"");
}

} // namespace bsgo
