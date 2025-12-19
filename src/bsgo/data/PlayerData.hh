
#pragma once

#include "Uuid.hh"
#include <optional>
#include <string>

namespace bsgo {

struct PlayerData
{
  Uuid dbId{};
  std::string name{};
  bool isAi{};

  /// @brief - This parameter defines the ship that this player might be attached
  /// to due to its role. This is currently only used for the Gunner role.
  std::optional<Uuid> attachedShip{};

  bool operator==(const PlayerData &rhs) const;

  auto serialize(std::ostream &out) const -> std::ostream &;
  bool deserialize(std::istream &in);
};

} // namespace bsgo
