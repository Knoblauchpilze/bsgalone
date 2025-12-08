
#include "GameRole.hh"
#include <stdexcept>

namespace bsgo {

auto str(const GameRole role) -> std::string
{
  switch (role)
  {
    case GameRole::PILOT:
      return "pilot";
    case GameRole::GUNNER:
      return "gunner";
    default:
      return "unknown";
  }
}

auto toDbGameRole(const GameRole role) -> std::string
{
  return str(role);
}

auto fromDbGameRole(const std::string &dbGameRole) -> GameRole
{
  if ("pilot" == dbGameRole)
  {
    return GameRole::PILOT;
  }
  if ("gunner" == dbGameRole)
  {
    return GameRole::GUNNER;
  }

  throw std::invalid_argument("Unknown game role \"" + dbGameRole + "\"");
}

} // namespace bsgo
