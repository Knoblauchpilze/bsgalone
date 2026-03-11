
#pragma once

#include "Uuid.hh"
#include <optional>
#include <string>

namespace bsgalone::core {

struct PlayerData
{
  Uuid dbId{};
  std::string name{};
  bool isAi{};

  /// @brief - This parameter defines the ship that this player might be attached
  /// to due to its role. This is currently only used for the Gunner role.
  std::optional<Uuid> attachedShip{};

  bool operator==(const PlayerData &rhs) const;
};

auto operator<<(std::ostream &out, const PlayerData &data) -> std::ostream &;
bool operator>>(std::istream &in, PlayerData &data);

} // namespace bsgalone::core
