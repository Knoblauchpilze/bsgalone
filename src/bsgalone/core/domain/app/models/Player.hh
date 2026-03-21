
#pragma once

#include "Faction.hh"
#include "GameRole.hh"
#include "Uuid.hh"
#include <optional>
#include <string>

namespace bsgalone::core {

struct Player
{
  Uuid dbId{};
  std::optional<Uuid> account{};
  std::string name{};
  Faction faction{};
  GameRole role{};
  Uuid systemDbId{};

  bool operator==(const Player &rhs) const;
};

} // namespace bsgalone::core
