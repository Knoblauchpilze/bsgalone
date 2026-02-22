
#pragma once

#include "EntityKind.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgalone::core {

/// @brief - This data class groups properties needed to define a target.
/// To communicate a target from the server to client applications, it is
/// needed to transmit both the target DB identifier and its kind. The
/// target component works with entity identifiers which can't reliably be
/// communicated between server/clients.
struct Target
{
  /// @brief - Defines the DB identifier of the source. This represents
  /// the game entity which has the `target` entity as target.
  bsgo::Uuid sourceDbId{};

  /// @brief - Defines the kind of the source entity: this helps interpreting
  /// the DB identifier and distinguish between entity types.
  EntityKind sourceKind{};

  /// @brief - The DB identifier of the target. It is an optional because
  /// this data class can be used in several different contexts:
  ///   - when the client applications is in a loading transition and needs
  ///     to receive the targets associated to the entities present in the
  ///     system. In this case, targetDbId is always set
  ///   - when a player tries to click on an entity and needs to acquire a
  ///     target. In this case, it can contain a target or be empty in case
  ///     the player clicked somewhere where there's no entity
  ///   - when a bot picks a target in the game (such as  finding a player).
  ///     In such cases, the target is set
  ///   - when a target is cleared from an entity: this can happen when an
  ///     entity dies or jumps to another system
  std::optional<bsgo::Uuid> targetDbId{};

  /// @brief - Defines the kind of the target entity. Plays a similar role
  /// as `sourceKind`. Also note that this value contains a value at the
  /// same time as `targetDbId`: either both are empty or contain a value.
  std::optional<EntityKind> targetKind{};

  bool operator==(const Target &rhs) const;

  auto serialize(std::ostream &out) const -> std::ostream &;
  bool deserialize(std::istream &in);

  auto str() const -> std::string;
};

} // namespace bsgalone::core
