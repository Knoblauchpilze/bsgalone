
#include "JumpService.hh"
#include "JumpUtils.hh"

namespace bsgo {

JumpService::JumpService(const Repositories &repositories,
                         CoordinatorShPtr coordinator,
                         const DatabaseEntityMapper &entityMapper)
  : AbstractService("jump", repositories)
  , m_coordinator(std::move(coordinator))
  , m_entityMapper(entityMapper)
{}

bool JumpService::tryRegisterJump(const Uuid shipDbId, const Uuid system) const
{
  const auto maybeShipEntity = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeShipEntity)
  {
    return false;
  }

  auto ship         = m_repositories.playerShipRepository->findOneById(shipDbId);
  const auto status = canShipJump(ship, system);
  if (JumpCompletionStatus::OK != status)
  {
    warn("Failed to process jump request for ship " + str(ship.id), str(status));
    return false;
  }

  ship.jumpSystem = system;
  m_repositories.playerShipRepository->save(ship);

  info("Registered jump to " + str(system) + " for ship " + str(shipDbId));

  auto playerShip              = m_coordinator->getEntity(*maybeShipEntity);
  const auto currentShipStatus = playerShip.statusComp().status();

  const auto newStatus = updateStatusForJump(currentShipStatus);
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

bool JumpService::tryCancelJump(const Uuid shipDbId) const
{
  const auto maybeShipEntity = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeShipEntity)
  {
    return false;
  }

  auto ship = m_repositories.playerShipRepository->findOneById(shipDbId);
  if (!canShipCancelJump(ship))
  {
    return false;
  }

  ship.jumpSystem.reset();
  m_repositories.playerShipRepository->save(ship);

  info("Cancelled jump for ship " + str(shipDbId));

  auto playerShip   = m_coordinator->getEntity(*maybeShipEntity);
  const auto status = playerShip.statusComp().status();

  const auto newStatus = updateStatusAfterJumpCancellation(status);
  playerShip.statusComp().setStatus(newStatus);

  return true;
}

} // namespace bsgo
