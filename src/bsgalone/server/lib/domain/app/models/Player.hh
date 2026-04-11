
#pragma once

#include "Faction.hh"
#include "GameRole.hh"
#include "Uuid.hh"
#include <optional>
#include <string>

namespace bsgalone::server {

struct Player
{
  core::Uuid dbId{};
  std::optional<core::Uuid> account{};
  std::string name{};
  core::Faction faction{};
  core::GameRole role{};

  bool operator==(const Player &rhs) const;
};

} // namespace bsgalone::server
