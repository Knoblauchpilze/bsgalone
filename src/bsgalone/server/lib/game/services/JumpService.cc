
#include "JumpService.hh"
#include "JumpUtils.hh"

namespace bsgalone::server {

JumpService::JumpService(const core::Repositories &repositories,
                         core::CoordinatorShPtr coordinator,
                         const core::DatabaseEntityMapper &entityMapper)
  : AbstractService("jump", repositories)
  , m_coordinator(std::move(coordinator))
  , m_entityMapper(entityMapper)
{}

bool JumpService::tryRegisterJump(const core::Uuid shipDbId, const core::Uuid system) const
{
  const auto maybeShipEntity = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeShipEntity)
  {
    return false;
  }

  auto ship         = m_repositories.playerShipRepository->findOneById(shipDbId);
  const auto status = canShipJump(ship, system);
  if (core::JumpCompletionStatus::OK != status)
  {
    warn("Failed to process jump request for ship " + core::str(ship.id), str(status));
    return false;
  }

  m_repositories.playerShipRepository->saveJump(shipDbId, system);

  info("Registered jump to " + core::str(system) + " for ship " + core::str(shipDbId));

  auto playerShip              = m_coordinator->getEntity(*maybeShipEntity);
  const auto currentShipStatus = playerShip.statusComp().status();

  const auto newStatus = updateStatusForJump(currentShipStatus);
  playerShip.statusComp().setStatus(newStatus);

  return true;
}

bool JumpService::tryCancelJump(const core::Uuid shipDbId) const
{
  const auto maybeShipEntity = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeShipEntity)
  {
    return false;
  }

  auto ship         = m_repositories.playerShipRepository->findOneById(shipDbId);
  const auto status = canShipCancelJump(ship);
  if (core::JumpCompletionStatus::OK != status)
  {
    warn("Failed to process jump cancellation for ship " + core::str(ship.id), str(status));
    return false;
  }

  m_repositories.playerShipRepository->saveJump(shipDbId, {});

  info("Cancelled jump for ship " + core::str(shipDbId));

  auto playerShip       = m_coordinator->getEntity(*maybeShipEntity);
  const auto shipStatus = playerShip.statusComp().status();

  const auto newStatus = updateStatusAfterJumpCancellation(shipStatus);
  playerShip.statusComp().setStatus(newStatus);

  return true;
}

} // namespace bsgalone::server
