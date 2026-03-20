
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
  auto findOneById(const Uuid player) const -> Player;
  auto findOneByAccount(const Uuid account) const -> Player override;
  auto findSystemByPlayer(const Uuid player) const -> Uuid;

  void save(const Player &player);
};

using PlayerRepositoryShPtr = std::shared_ptr<PlayerRepository>;

} // namespace bsgalone::core
