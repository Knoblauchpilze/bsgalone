
#pragma once

#include "AbstractRepository.hh"
#include "GameRole.hh"
#include "Uuid.hh"
#include <memory>
#include <optional>

namespace bsgo {

struct PlayerRole
{
  Uuid player{};
  GameRole role{};
  std::optional<Uuid> targetShip{};
};

class PlayerRoleRepository : public AbstractRepository
{
  public:
  PlayerRoleRepository(const DbConnectionShPtr &connection);
  ~PlayerRoleRepository() override = default;

  void initialize() override;

  auto findOneByPlayer(const Uuid player) const -> PlayerRole;

  void save(const PlayerRole &playerRole);
};

using PlayerRoleRepositoryShPtr = std::shared_ptr<PlayerRoleRepository>;

} // namespace bsgo
