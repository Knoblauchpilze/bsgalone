
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
  /// the player's data as a new entity in the database. In case there's already a
  /// player attached to the account, its name will be overridden by the value defined
  /// in the input of this function.
  /// Additionally, player role will be inserted in the database. In case the player
  /// already has a role, it will be updated.
  /// The identifier of the player is ignored and will be returned in the output model.
  /// @param player - the player to save
  /// @return - an up to date version of the player as saved in the database
  auto save(Player player) -> Player override;
};

using PlayerRepositoryShPtr = std::shared_ptr<PlayerRepository>;

} // namespace bsgalone::server
