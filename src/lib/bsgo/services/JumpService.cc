
#include "JumpService.hh"

namespace bsgo {

JumpService::JumpService(const Repositories &repositories, const CoordinatorShPtr &coordinator)
  : AbstractService("jump", repositories, coordinator)
{}

namespace {
bool isShipActive(const PlayerShip &ship)
{
  return ship.player.has_value() && ship.active;
}

bool canShipJump(const PlayerShip &ship,
                 const std::optional<Uuid> &currentSystem,
                 const Uuid &newSystem)
{
  return isShipActive(ship) && !ship.jumpSystem.has_value() && currentSystem.has_value()
         && *currentSystem != newSystem;
}

} // namespace

bool JumpService::tryRegisterJump(const Uuid &shipDbId,
                                  const Uuid &shipEntityId,
                                  const Uuid &system) const
{
  auto ship             = m_repositories.playerShipRepository->findOneById(shipDbId);
  const auto shipSystem = m_repositories.playerShipRepository->findSystemById(shipDbId);
  if (!canShipJump(ship, shipSystem, system))
  {
    return false;
  }

  ship.jumpSystem = system;
  m_repositories.playerShipRepository->save(ship);

  info("Registered jump to " + str(system) + " for ship " + str(shipDbId));

  auto playerShip   = m_coordinator->getEntity(shipEntityId);
  const auto status = playerShip.statusComp().status();

  const auto newStatus = updateStatusForJump(status);
  playerShip.statusComp().setStatus(newStatus);

  return true;
}

namespace {
bool canShipCancelJump(const PlayerShip &ship)
{
  return isShipActive(ship) && ship.jumpSystem.has_value();
}
} // namespace

bool JumpService::tryCancelJump(const Uuid &shipDbId, const Uuid &shipEntityId) const
{
  auto ship = m_repositories.playerShipRepository->findOneById(shipDbId);
  if (!canShipCancelJump(ship))
  {
    return false;
  }

  ship.jumpSystem.reset();
  m_repositories.playerShipRepository->save(ship);

  info("Cancelled jump for ship " + str(shipDbId));

  auto playerShip   = m_coordinator->getEntity(shipEntityId);
  const auto status = playerShip.statusComp().status();

  const auto newStatus = updateStatusAfterJumpCancellation(status);
  playerShip.statusComp().setStatus(newStatus);

  return true;
}

namespace {
bool canShipCompleteJump(const PlayerShip &ship, const std::optional<Uuid> &currentSystem)
{
  return isShipActive(ship) && ship.jumpSystem.has_value() && currentSystem.has_value()
         && *currentSystem != *ship.jumpSystem;
}
} // namespace

bool JumpService::tryJump(const Uuid &shipDbId, const Uuid &shipEntityId) const
{
  auto ship             = m_repositories.playerShipRepository->findOneById(shipDbId);
  const auto shipSystem = m_repositories.playerShipRepository->findSystemById(shipDbId);
  if (!canShipCompleteJump(ship, shipSystem))
  {
    return false;
  }

  const auto system = m_repositories.systemRepository->findOneById(*ship.jumpSystem);

  m_repositories.systemRepository->updateSystemForShip(ship.id, *ship.jumpSystem);
  ship.jumpSystem.reset();
  m_repositories.playerShipRepository->save(ship);

  info("Completed jump to " + system.name + " for " + str(shipDbId));

  auto playerShip = m_coordinator->getEntity(shipEntityId);
  playerShip.statusComp().setStatus(Status::APPEARING);
  playerShip.statusComp().resetAppearingTime();

  return true;
}

} // namespace bsgo
