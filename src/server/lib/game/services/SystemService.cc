
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

  // Do not distribute resources to players not having an account linked to them.
  // Those players are used to represent bots and should not receive resources.
  if (!player.account)
  {
    debug("Ignoring distributing resource to " + str(playerDbId) + " as it has no linked account");
    return true;
  }

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
bool canShipBeDisposedOf(const PlayerShip &ship)
{
  return ship.active;
}
} // namespace

bool SystemService::disposeOfPlayerShip(const Uuid shipDbId, const bool dead) const
{
  auto ship = m_repositories.playerShipRepository->findOneById(shipDbId);

  if (!canShipBeDisposedOf(ship))
  {
    warn("Failed to dispose of ship " + str(shipDbId),
         std::string("Ship ") + (ship.active ? "is" : "is not") + " active and "
           + (ship.docked ? "is" : "is not") + " docked");
    return false;
  }

  const auto player = m_repositories.playerRepository->findOneById(ship.player);

  if (player.account)
  {
    return disposeOfPlayerShip(ship, dead);
  }
  else
  {
    return disposeOfAiShip(ship, dead);
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
  ship.position    = Eigen::Vector3f::Zero();
  ship.powerPoints = 0.0f;

  m_repositories.systemRepository->updateSystemForShip(ship.id, *ship.system, ship.docked);
  m_repositories.playerShipRepository->save(ship);
  m_repositories.playerShipRepository->saveJump(ship.id, {});

  // Reset the role so that the spot on a ship might be made available to other
  // players.
  auto playerRole = m_repositories.playerRoleRepository->findOneByPlayer(playerDbId);
  playerRole.targetShip.reset();
  m_repositories.playerRoleRepository->save(playerRole);

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

bool SystemService::registerAiBehaviorMilestone(const Uuid shipDbId, const int targetReached) const
{
  AiBehavior behavior{.ship = shipDbId, .targetIndex = targetReached};

  m_repositories.aiBehaviorRepository->save(behavior);

  return true;
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

bool SystemService::disposeOfAiShip(const PlayerShip &playerShip, const bool dead) const
{
  if (!playerShip.system)
  {
    error("AI ship " + str(playerShip.id) + " has no system");
  }
  if (!dead)
  {
    error("Cannot dispose of AI ship " + str(playerShip.id), "Ship is not dead");
  }

  const auto tickData = m_repositories.tickRepository->findOneBySystem(*playerShip.system);

  const auto deathTick   = tickData.currentTick;
  const auto respawnTick = deathTick + AI_SHIP_RESPAWN_TIME_IN_TICKS;

  m_repositories.playerShipRepository->saveRespawn(playerShip.id, deathTick, respawnTick);

  return true;
}

bool SystemService::disposeOfPlayerShip(PlayerShip playerShip, const bool dead) const
{
  playerShip.docked = true;
  playerShip.jumpSystem.reset();
  if (dead)
  {
    playerShip.position = Eigen::Vector3f::Zero();
  }
  playerShip.powerPoints = 0.0f;

  m_repositories.systemRepository->updateSystemForShip(playerShip.id,
                                                       *playerShip.system,
                                                       playerShip.docked);
  m_repositories.playerShipRepository->save(playerShip);
  // Cancel jump to another system.
  m_repositories.playerShipRepository->saveJump(playerShip.id, {});

  return true;
}

} // namespace bsgo
