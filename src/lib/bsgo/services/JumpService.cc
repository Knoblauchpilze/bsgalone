
#include "JumpService.hh"

namespace bsgo {

JumpService::JumpService(const Repositories &repositories)
  : AbstractService("jump", repositories)
{}

namespace {
bool isShipActive(const PlayerShip &ship)
{
  return ship.player.has_value() && ship.active;
}

bool canShipJump(const PlayerShip &ship)
{
  return isShipActive(ship) && !ship.jumpSystem.has_value();
}

} // namespace

bool JumpService::tryRegisterJump(const Uuid &shipId, const Uuid &system) const
{
  auto ship = m_repositories.playerShipRepository->findOneById(shipId);
  if (!canShipJump(ship))
  {
    return false;
  }

  ship.jumpSystem = system;
  m_repositories.playerShipRepository->save(ship);

  info("Registered jump to " + str(system) + " for ship " + str(shipId));

  return true;
}

namespace {
bool canShipCancelJump(const PlayerShip &ship)
{
  return isShipActive(ship) && ship.jumpSystem.has_value();
}
} // namespace

bool JumpService::tryCancelJump(const Uuid &shipId) const
{
  auto ship = m_repositories.playerShipRepository->findOneById(shipId);
  if (!canShipCancelJump(ship))
  {
    return false;
  }

  ship.jumpSystem.reset();
  m_repositories.playerShipRepository->save(ship);

  info("Cancelled jump for ship " + str(shipId));

  return true;
}

namespace {
bool canShipCompleteJump(const PlayerShip &ship)
{
  // Same conditions.
  return canShipCancelJump(ship);
}
} // namespace

bool JumpService::tryJump(const Uuid &shipId) const
{
  const auto ship = m_repositories.playerShipRepository->findOneById(shipId);
  if (!canShipCompleteJump(ship))
  {
    return false;
  }

  warn("should try to jump " + str(shipId));
  return true;
}

} // namespace bsgo
