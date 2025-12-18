
#include "PlayerService.hh"

namespace bsgo {

PlayerService::PlayerService(const Repositories &repositories)
  : AbstractService("player", repositories)
{}

bool PlayerService::tryJoinShip(const Uuid playerDbId, const Uuid shipDbId) const
{
  return withSafetyNet([this, playerDbId, shipDbId]() { makePlayerJoinShip(playerDbId, shipDbId); },
                       "tryJoinShip");
}

void PlayerService::makePlayerJoinShip(const Uuid playerDbId, const Uuid shipDbId) const
{
  auto roleData       = m_repositories.playerRoleRepository->findOneByPlayer(playerDbId);
  roleData.targetShip = shipDbId;
  m_repositories.playerRoleRepository->save(roleData);

  info("Player " + str(playerDbId) + " joined ship " + str(shipDbId));
}

} // namespace bsgo
