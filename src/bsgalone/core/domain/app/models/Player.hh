
#pragma once

#include "Faction.hh"
#include "GameRole.hh"
#include "PlayerShip.hh"
#include "Uuid.hh"
#include <optional>
#include <string>
#include <vector>

namespace bsgalone::core {

struct Player
{
  Uuid dbId{};
  std::optional<Uuid> account{};
  std::string name{};
  Faction faction{};
  GameRole role{};

  std::vector<PlayerShip> ships{};

  bool operator==(const Player &rhs) const;

  auto systemDbId() const -> Uuid;
};

} // namespace bsgalone::core
