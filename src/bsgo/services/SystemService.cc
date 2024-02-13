
#include "SystemService.hh"

namespace bsgo {

SystemService::SystemService(const Repositories &repositories)
  : AbstractService("system", repositories)
{}

bool SystemService::tryDistributeResource(const Uuid playerDbId,
                                          const Uuid resourceDbId,
                                          const float amount) const
{
  const auto player = m_repositories.playerRepository->findOneById(playerDbId);

  const auto existing = findExistingResourceAmount(playerDbId, resourceDbId);
  const auto updated  = existing + amount;
  if (updated < 0.0f)
  {
    return false;
  }

  PlayerResource data{.player = playerDbId, .resource = resourceDbId, .amount = updated};
  m_repositories.playerResourceRepository->save(data);

  return true;
}

namespace {
bool canShipBeSentBackToOutpost(const PlayerShip &ship)
{
  return ship.active && !ship.docked;
}
} // namespace

bool SystemService::trySendPlayerShipBackToOutpost(const Uuid shipDbId) const
{
  auto ship = m_repositories.playerShipRepository->findOneById(shipDbId);

  if (!canShipBeSentBackToOutpost(ship))
  {
    return false;
  }

  ship.docked = true;
  ship.jumpSystem.reset();

  m_repositories.systemRepository->updateSystemForShip(ship.id, *ship.system, ship.docked);
  m_repositories.playerShipRepository->save(ship);

  return true;
}

auto SystemService::trySendPlayerBackToOutpost(const Uuid &playerDbId) const -> ForcedDockResult
{
  auto ship = m_repositories.playerShipRepository->findOneByPlayerAndActive(playerDbId);

  ForcedDockResult out{.alreadyDocked = ship.docked};

  ship.docked = true;
  ship.jumpSystem.reset();

  m_repositories.systemRepository->updateSystemForShip(ship.id, *ship.system, ship.docked);
  m_repositories.playerShipRepository->save(ship);

  return out;
}

namespace {
bool isShipActive(const PlayerShip &ship)
{
  return ship.player.has_value() && ship.active;
}

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

auto SystemService::tryJump(const Uuid shipDbId) const -> JumpResult
{
  JumpResult out{};

  auto ship = m_repositories.playerShipRepository->findOneById(shipDbId);
  if (!canShipCompleteJump(ship))
  {
    return out;
  }

  out.sourceSystem      = *ship.system;
  out.destinationSystem = *ship.jumpSystem;

  const auto system = m_repositories.systemRepository->findOneById(*ship.jumpSystem);

  m_repositories.systemRepository->updateSystemForShip(ship.id, *ship.jumpSystem, ship.docked);
  ship.jumpSystem.reset();
  m_repositories.playerShipRepository->save(ship);

  info("Completed jump to " + system.name + " for " + str(shipDbId));
  out.success = true;

  return out;
}

auto SystemService::getSystemDbIdForAsteroid(const Uuid asteroidDbId) const -> Uuid
{
  return m_repositories.asteroidRepository->findOneById(asteroidDbId).system;
}

auto SystemService::tryGetSystemDbIdForShip(const Uuid shipDbId) const -> std::optional<Uuid>
{
  return m_repositories.playerShipRepository->findOneById(shipDbId).system;
}

auto SystemService::getShipDbIdForPlayer(const Uuid playerDbId) const -> Uuid
{
  const auto playerShip = m_repositories.playerShipRepository->findOneByPlayerAndActive(playerDbId);
  return playerShip.id;
}

auto SystemService::findExistingResourceAmount(const Uuid playerDbId, const Uuid resourceDbId) const
  -> float
{
  const auto resources = m_repositories.playerResourceRepository->findAllByPlayer(playerDbId);
  for (const auto &resource : resources)
  {
    if (resource.resource == resourceDbId)
    {
      return resource.amount;
    }
  }

  return 0.0f;
}

} // namespace bsgo
