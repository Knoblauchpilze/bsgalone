
#pragma once

#include "AbstractRepository.hh"
#include "ForManagingPlayer.hh"
#include "Player.hh"
#include "Uuid.hh"
#include <memory>
#include <optional>
#include <unordered_set>

namespace bsgalone::server {

class PlayerRepository : public ForManagingPlayer, public AbstractRepository
{
  public:
  PlayerRepository(DbConnectionShPtr connection);
  ~PlayerRepository() override = default;

  void initialize() override;

  auto findOneById(const core::Uuid playerDbId) const -> Player override;
  auto findOneByAccount(const core::Uuid accountDbId) const -> Player override;

  /// @brief - Saves the player to the database. This function will attempt to insert
  /// the player's data as a new entity in the database. An entry for the role of the
  /// player will be created.
  /// In case there's already a player with the same identifier, it will be updated:
  //   - the name will be updated
  //   - the game role will be updated
  /// Additionally, player role will be inserted in the database. In case the player
  /// already has a role, it will be updated.
  /// @param player - the player to save
  void save(Player player) const override;
};

using PlayerRepositoryShPtr = std::shared_ptr<PlayerRepository>;

} // namespace bsgalone::server
