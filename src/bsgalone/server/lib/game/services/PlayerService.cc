
#include "PlayerService.hh"

namespace bsgalone::server {

PlayerService::PlayerService(const core::Repositories &repositories)
  : AbstractService("player", repositories)
{}

bool PlayerService::tryJoinShip(const core::Uuid playerDbId, const core::Uuid shipDbId) const
{
  return withSafetyNet([this, playerDbId, shipDbId]() { makePlayerJoinShip(playerDbId, shipDbId); },
                       "tryJoinShip");
}

void PlayerService::makePlayerJoinShip(const core::Uuid playerDbId, const core::Uuid shipDbId) const
{
  auto roleData       = m_repositories.playerRoleRepository->findOneByPlayer(playerDbId);
  roleData.targetShip = shipDbId;
  m_repositories.playerRoleRepository->save(roleData);

  info("Player " + core::str(playerDbId) + " joined ship " + core::str(shipDbId));
}

} // namespace bsgalone::server
