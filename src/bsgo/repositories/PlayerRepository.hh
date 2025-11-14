
#pragma once

#include "AbstractRepository.hh"
#include "Faction.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <optional>
#include <unordered_set>

namespace bsgo {

struct Player
{
  Uuid id{};
  std::optional<Uuid> account{};
  std::string name{};
  Faction faction{};
};

class PlayerRepository : public AbstractRepository
{
  public:
  PlayerRepository(const DbConnectionShPtr &connection);
  ~PlayerRepository() override = default;

  void initialize() override;

  auto findAll() const -> std::unordered_set<Uuid>;
  auto findAllUndockedBySystem(const Uuid system) const -> std::unordered_set<Uuid>;
  auto findOneById(const Uuid player) const -> Player;
  auto findOneByName(const std::string &name) const -> std::optional<Player>;
  auto findSystemByPlayer(const Uuid player) const -> Uuid;

  void save(const Player &player);
};

using PlayerRepositoryShPtr = std::shared_ptr<PlayerRepository>;

} // namespace bsgo
