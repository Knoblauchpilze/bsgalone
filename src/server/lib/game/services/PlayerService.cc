
#include "PlayerService.hh"

namespace bsgo {

PlayerService::PlayerService(const Repositories &repositories)
  : AbstractService("player", repositories)
{}

bool PlayerService::tryJoinShip(const Uuid playerDbId, const Uuid shipDbId) const
{
  // TODO: Handle join ship
  warn("should handle player " + str(playerDbId) + " joining ship " + str(shipDbId));
  return false;
}

} // namespace bsgo
