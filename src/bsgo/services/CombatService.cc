
#include "CombatService.hh"

namespace bsgo {

CombatService::CombatService(const Repositories &repositories)
  : AbstractService("combat", repositories)
{}

bool CombatService::tryDistributeResource(const Uuid playerDbId,
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

bool CombatService::trySendPlayerShipBackToOutpost(const Uuid shipDbId) const
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

auto CombatService::getSystemDbIdForAsteroid(const Uuid asteroidDbId) const -> Uuid
{
  return m_repositories.asteroidRepository->findOneById(asteroidDbId).system;
}

auto CombatService::tryGetSystemDbIdForShip(const Uuid shipDbId) const -> std::optional<Uuid>
{
  return m_repositories.playerShipRepository->findOneById(shipDbId).system;
}

auto CombatService::findExistingResourceAmount(const Uuid playerDbId, const Uuid resourceDbId) const
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
