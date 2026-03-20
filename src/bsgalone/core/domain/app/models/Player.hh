
#pragma once

#include "Faction.hh"
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

  bool operator==(const Player &rhs) const;
};

} // namespace bsgalone::core
