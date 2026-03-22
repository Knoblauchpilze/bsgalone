
#pragma once

#include "AbstractRepository.hh"
#include "ForManagingPlayer.hh"
#include "Player.hh"
#include "Uuid.hh"
#include <memory>
#include <optional>
#include <unordered_set>

namespace bsgalone::core {

class PlayerRepository : public AbstractRepository, public ForManagingPlayer
{
  public:
  PlayerRepository(const DbConnectionShPtr &connection);
  ~PlayerRepository() override = default;

  void initialize() override;

  auto findAll() const -> std::unordered_set<Uuid>;
  auto findAllBySystem(const Uuid system) const -> std::unordered_set<Uuid>;
  auto findAllUndockedBySystem(const Uuid system) const -> std::unordered_set<Uuid>;
  auto findSystemByPlayer(const Uuid player) const -> Uuid;

  auto findOneById(const Uuid playerDbId) const -> Player override;
  auto findOneByAccount(const Uuid accountDbId) const -> Player override;
  auto save(Player player) -> Player override;
};

using PlayerRepositoryShPtr = std::shared_ptr<PlayerRepository>;

} // namespace bsgalone::core
