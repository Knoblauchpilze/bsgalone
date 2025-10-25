
#include "SystemService.hh"
#include "JumpUtils.hh"

namespace bsgo {

SystemService::SystemService(const Repositories &repositories)
  : AbstractService("system", repositories)
{}

bool SystemService::tryDistributeResource(const Uuid playerDbId,
                                          const Uuid resourceDbId,
                                          const int amount) const
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

bool SystemService::disposeOfDeadShip(const Uuid shipDbId) const
{
  auto ship = m_repositories.playerShipRepository->findOneById(shipDbId);

  if (!canShipBeSentBackToOutpost(ship))
  {
    warn("Failed to send ship " + str(shipDbId) + " back to outpost",
         std::string("Ship ") + (ship.active ? "is" : "is not") + " active and "
           + (ship.docked ? "is" : "is not") + "docked");
    return false;
  }

  if (ship.player)
  {
    return disposeOfDeadPlayerShip(ship);
  }
  else
  {
    return disposeOfDeadAiShip(ship);
  }
}

auto SystemService::sendPlayerBackToOutpost(const Uuid &playerDbId) const -> ForcedDockResult
{
  auto ship = m_repositories.playerShipRepository->findOneByPlayerAndActive(playerDbId);

  ForcedDockResult out{.alreadyDocked = ship.docked};

  ship.docked = true;
  ship.jumpSystem.reset();
  // Reset ship position as this is used in the logout flow: we want the player to
  // respawn at the initial position.
  ship.position = Eigen::Vector3f::Zero();

  m_repositories.systemRepository->updateSystemForShip(ship.id, *ship.system, ship.docked);
  m_repositories.playerShipRepository->save(ship);
  m_repositories.playerShipRepository->saveJump(ship.id, {});

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

  // Override ship position to reset the position in the new system
  ship.position = Eigen::Vector3f::Zero();

  const auto system = m_repositories.systemRepository->findOneById(*ship.jumpSystem);

  m_repositories.systemRepository->updateSystemForShip(ship.id, *ship.jumpSystem, ship.docked);
  m_repositories.playerShipRepository->save(ship);
  m_repositories.playerShipRepository->saveJump(ship.id, {});

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
  return m_repositories.systemOutpostRepository->findOneById(outpostDbId).system;
}

auto SystemService::getShipDbIdForPlayer(const Uuid playerDbId) const -> Uuid
{
  const auto playerShip = m_repositories.playerShipRepository->findOneByPlayerAndActive(playerDbId);
  return playerShip.id;
}

namespace {
const auto ASTEROID_RESPAWN_TIME_IN_TICKS = chrono::TickDuration::fromInt(10);
}

bool SystemService::tryMarkAsteroidForRespawn(const Uuid asteroidDbId) const
{
  // There's no need to check the health or any other preconditions to
  // mark the asteroid as ready for respawn because this is triggered
  // by the processing of a EntityRemovedMessage in the server. It is
  // therefore assumed that the process is validated ahead of time.
  const auto asteroid = m_repositories.asteroidRepository->findOneById(asteroidDbId);
  const auto tickData = m_repositories.tickRepository->findOneBySystem(asteroid.system);

  const auto deathTick   = tickData.currentTick;
  const auto respawnTick = deathTick + ASTEROID_RESPAWN_TIME_IN_TICKS;

  m_repositories.asteroidRepository->saveRespawn(asteroidDbId, deathTick, respawnTick);

  return true;
}

auto SystemService::findExistingResourceAmount(const Uuid playerDbId, const Uuid resourceDbId) const
  -> int
{
  const auto resources = m_repositories.playerResourceRepository->findAllByPlayer(playerDbId);
  for (const auto &resource : resources)
  {
    if (resource.resource == resourceDbId)
    {
      return resource.amount;
    }
  }

  return 0;
}

namespace {
const auto AI_SHIP_RESPAWN_TIME_IN_TICKS = chrono::TickDuration::fromInt(10);
}

bool SystemService::disposeOfDeadAiShip(const PlayerShip &playerShip) const
{
  if (!playerShip.system)
  {
    error("AI ship " + str(playerShip.id) + " has no system");
  }

  const auto tickData = m_repositories.tickRepository->findOneBySystem(*playerShip.system);

  const auto deathTick   = tickData.currentTick;
  const auto respawnTick = deathTick + AI_SHIP_RESPAWN_TIME_IN_TICKS;

  m_repositories.playerShipRepository->saveRespawn(playerShip.id, deathTick, respawnTick);

  return true;
}

bool SystemService::disposeOfDeadPlayerShip(PlayerShip playerShip) const
{
  playerShip.docked = true;
  playerShip.jumpSystem.reset();

  m_repositories.systemRepository->updateSystemForShip(playerShip.id,
                                                       *playerShip.system,
                                                       playerShip.docked);
  m_repositories.playerShipRepository->save(playerShip);
  // Cancel jump to another system.
  m_repositories.playerShipRepository->saveJump(playerShip.id, {});

  return true;
}

} // namespace bsgo
