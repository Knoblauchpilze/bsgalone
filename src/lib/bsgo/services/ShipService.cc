
#include "ShipService.hh"

namespace bsgo {

ShipService::ShipService(const Repositories &repositories, const CoordinatorShPtr &coordinator)
  : AbstractService("ship", repositories, coordinator)
{}

bool ShipService::trySelectShip(const Uuid &shipDbId) const
{
  const auto newActiveShip = m_repositories.playerShipRepository->findOneById(shipDbId);
  if (!newActiveShip.player)
  {
    return false;
  }

  const auto currentActiveShip = m_repositories.playerShipRepository->findOneByPlayerAndActive(
    *newActiveShip.player);

  if (!currentActiveShip.player)
  {
    return false;
  }
  if (newActiveShip.active)
  {
    return false;
  }
  if (*currentActiveShip.player != *newActiveShip.player)
  {
    return false;
  }

  switchActiveShip(currentActiveShip, newActiveShip);
  switchShipSystem(currentActiveShip, newActiveShip);

  return true;
}

bool ShipService::tryDock(const Uuid &shipEntityId) const
{
  auto ship              = m_coordinator->getEntity(shipEntityId);
  const auto &statusComp = ship.statusComp();

  if (Status::DOCKED == statusComp.status())
  {
    return false;
  }

  ship.statusComp().setStatus(Status::DOCKED);

  return true;
}

bool ShipService::tryUndock(const Uuid &shipEntityId) const
{
  auto ship              = m_coordinator->getEntity(shipEntityId);
  const auto &statusComp = ship.statusComp();

  if (statusComp.status() != Status::DOCKED)
  {
    return false;
  }

  ship.statusComp().setStatus(Status::APPEARING);

  return true;
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
