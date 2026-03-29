
#pragma once

#include "AbstractRepository.hh"
#include "ForManagingPlayer.hh"
#include "Player.hh"
#include "Uuid.hh"
#include <memory>
#include <optional>
#include <unordered_set>

namespace bsgalone::core {

class PlayerRepository : public ForManagingPlayer, public AbstractRepository
{
  public:
  PlayerRepository(DbConnectionShPtr connection);
  ~PlayerRepository() override = default;

  void initialize() override;

  auto findOneById(const Uuid playerDbId) const -> Player override;
  auto findOneByAccount(const Uuid accountDbId) const -> Player override;
  auto save(Player player) -> Player override;

  private:
  auto enrichWithSystem(Player player) const -> Player;
};

using PlayerRepositoryShPtr = std::shared_ptr<PlayerRepository>;

} // namespace bsgalone::core
