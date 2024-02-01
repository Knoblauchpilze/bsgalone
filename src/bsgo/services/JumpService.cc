
#include "JumpService.hh"

namespace bsgo {

JumpService::JumpService(const Repositories &repositories,
                         CoordinatorShPtr coordinator,
                         const DatabaseEntityMapper &entityMapper)
  : AbstractService("jump", repositories)
  , m_coordinator(std::move(coordinator))
  , m_entityMapper(entityMapper)
{}

namespace {
bool isShipActive(const PlayerShip &ship)
{
  return ship.player.has_value() && ship.active;
}

bool canShipJump(const PlayerShip &ship, const Uuid newSystem)
{
  if (!isShipActive(ship))
  {
    return false;
  }
  if (!ship.system.has_value())
  {
    return false;
  }
  if (ship.jumpSystem.has_value())
  {
    return false;
  }
  if (*ship.system == newSystem)
  {
    return false;
  }
  if (ship.docked)
  {
    return false;
  }

  return true;
}

} // namespace

bool JumpService::tryRegisterJump(const Uuid shipDbId, const Uuid system) const
{
  const auto maybeShipEntity = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeShipEntity)
  {
    return false;
  }

  auto ship = m_repositories.playerShipRepository->findOneById(shipDbId);
  if (!canShipJump(ship, system))
  {
    return false;
  }

  ship.jumpSystem = system;
  m_repositories.playerShipRepository->save(ship);

  info("Registered jump to " + str(system) + " for ship " + str(shipDbId));

  auto playerShip   = m_coordinator->getEntity(*maybeShipEntity);
  const auto status = playerShip.statusComp().status();

  const auto newStatus = updateStatusForJump(status);
  playerShip.statusComp().setStatus(newStatus);

  return true;
}

namespace {
bool canShipCancelJump(const PlayerShip &ship)
{
  if (!isShipActive(ship))
  {
    return false;
  }
  if (!ship.jumpSystem.has_value())
  {
    return false;
  }
  if (ship.docked)
  {
    return false;
  }

  return true;
}
} // namespace

bool JumpService::tryCancelJump(const Uuid shipDbId, const Uuid shipEntityId) const
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
bool canShipCompleteJump(const PlayerShip &ship)
{
  if (!isShipActive(ship))
  {
    return false;
  }
  if (!ship.jumpSystem.has_value())
  {
    return false;
  }
  if (!ship.system.has_value())
  {
    return false;
  }
  if (*ship.jumpSystem == *ship.system)
  {
    return false;
  }
  if (ship.docked)
  {
    return false;
  }

  return true;
}
} // namespace

bool JumpService::tryJump(const Uuid shipDbId, const Uuid shipEntityId) const
{
  auto ship = m_repositories.playerShipRepository->findOneById(shipDbId);
  if (!canShipCompleteJump(ship))
  {
    return false;
  }

  const auto system = m_repositories.systemRepository->findOneById(*ship.jumpSystem);

  m_repositories.systemRepository->updateSystemForShip(ship.id, *ship.jumpSystem, ship.docked);
  ship.jumpSystem.reset();
  m_repositories.playerShipRepository->save(ship);

  info("Completed jump to " + system.name + " for " + str(shipDbId));

  auto playerShip = m_coordinator->getEntity(shipEntityId);
  playerShip.statusComp().setStatus(Status::APPEARING);
  playerShip.statusComp().resetAppearingTime();

  return true;
}

} // namespace bsgo
