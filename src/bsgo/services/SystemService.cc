
#include "SystemService.hh"
#include "JumpUtils.hh"

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
    warn("Failed to distribute " + std::to_string(amount) + " of " + str(resourceDbId) + " to "
           + str(playerDbId),
         "Conflict with existing amount " + std::to_string(existing));
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
    warn("Failed to send ship " + str(shipDbId) + " back to outpost",
         std::string("Ship ") + (ship.active ? "is" : "is not") + " active and "
           + (ship.docked ? "is" : "is not") + "docked");
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

auto SystemService::tryJump(const Uuid shipDbId) const -> JumpResult
{
  JumpResult out{};

  auto ship         = m_repositories.playerShipRepository->findOneById(shipDbId);
  const auto status = canShipCompleteJump(ship);
  if (JumpCompletionStatus::OK != status)
  {
    warn("Failed to process jump for ship " + str(ship.id), str(status));
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

auto SystemService::tryGetSystemDbIdForShip(const Uuid shipDbId) const -> std::optional<Uuid>
{
  return m_repositories.playerShipRepository->findOneById(shipDbId).system;
}

auto SystemService::getSystemDbIdForAsteroid(const Uuid asteroidDbId) const -> Uuid
{
  return m_repositories.asteroidRepository->findOneById(asteroidDbId).system;
}

auto SystemService::getSystemDbIdForOutpost(const Uuid outpostDbId) const -> Uuid
{
  return m_repositories.outpostRepository->findOneById(outpostDbId).system;
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
