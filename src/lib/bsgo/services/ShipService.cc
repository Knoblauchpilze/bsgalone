
#include "ShipService.hh"

namespace bsgo {

ShipService::ShipService(const Repositories &repositories)
  : AbstractService("ship", repositories)
{}

void ShipService::setPlayerDbId(const Uuid &player)
{
  m_playerDbId = player;
}

bool ShipService::isReady() const noexcept
{
  return m_playerDbId.has_value();
}

bool ShipService::trySelectShip(const Uuid &playerShipId) const
{
  checkPlayerDbIdExists();

  const auto currentActiveShip = m_repositories.playerShipRepository->findOneByPlayerAndActive(
    *m_playerDbId);
  const auto newActiveShip = m_repositories.playerShipRepository->findOneById(playerShipId);
  if (!verifyPreconditions(newActiveShip))
  {
    return false;
  }

  switchActiveShip(currentActiveShip, newActiveShip);
  switchShipSystem(currentActiveShip, newActiveShip);

  return true;
}

void ShipService::checkPlayerDbIdExists() const
{
  if (!m_playerDbId)
  {
    error("Expected player db id to exist but it does not");
  }
}

bool ShipService::verifyPreconditions(const PlayerShip &ship) const
{
  return !ship.active && *m_playerDbId == ship.player;
}

void ShipService::switchActiveShip(PlayerShip currentActiveShip, PlayerShip newActiveShip) const
{
  currentActiveShip.active = false;
  newActiveShip.active     = true;

  m_repositories.playerShipRepository->save(currentActiveShip);
  m_repositories.playerShipRepository->save(newActiveShip);
}

void ShipService::switchShipSystem(const PlayerShip &currentActiveShip,
                                   const PlayerShip &newActiveShip) const
{
  m_repositories.systemRepository->updateShipForSystem(currentActiveShip.id, newActiveShip.id);
}

} // namespace bsgo
