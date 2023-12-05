
#pragma once

#include "AbstractRepository.hh"
#include "Faction.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <optional>

namespace bsgo {

struct Player
{
  Uuid id{};
  std::string name{};
  std::string password{};
  Faction faction{};
};

class PlayerRepository : public AbstractRepository
{
  public:
  PlayerRepository(const DbConnectionShPtr &connection);
  ~PlayerRepository() override = default;

  auto findOneByName(const std::string &name) const -> std::optional<Player>;
  auto findSystemByPlayer(const Uuid &player) const -> Uuid;
};

using PlayerRepositoryShPtr = std::shared_ptr<PlayerRepository>;

} // namespace bsgo
