
#include "ShipMessageConsumer.hh"

namespace pge {

ShipMessageConsumer::ShipMessageConsumer(const bsgo::DatabaseEntityMapper &entityMapper,
                                         bsgo::CoordinatorShPtr coordinator)
  : bsgo::AbstractMessageConsumer("ship",
                                  {bsgo::MessageType::JUMP_REQUESTED,
                                   bsgo::MessageType::JUMP_CANCELLED,
                                   bsgo::MessageType::TARGET})
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

void ShipMessageConsumer::onMessageReceived(const bsgo::IMessage &message)
{
  switch (message.type())
  {
    case bsgo::MessageType::JUMP_REQUESTED:
      handleJumpRequested(message.as<bsgo::JumpRequestedMessage>());
      break;
    case bsgo::MessageType::JUMP_CANCELLED:
      handleJumpCancelled(message.as<bsgo::JumpCancelledMessage>());
      break;
    case bsgo::MessageType::TARGET:
      handleTargetAcquired(message.as<bsgo::TargetMessage>());
      break;
    default:
      error("Unsupported message type " + bsgo::str(message.type()));
  }
}

void ShipMessageConsumer::handleJumpRequested(const bsgo::JumpRequestedMessage &message) const
{
  const auto shipDbId = message.getShipDbId();

  const auto maybeShip = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeShip)
  {
    warn("Failed to process jump requested message for ship " + bsgo::str(shipDbId));
    return;
  }

  auto ship         = m_coordinator->getEntity(*maybeShip);
  const auto status = ship.statusComp().status();

  const auto newStatus = bsgo::updateStatusForJump(status);
  ship.statusComp().setStatus(newStatus);
}

void ShipMessageConsumer::handleJumpCancelled(const bsgo::JumpCancelledMessage &message) const
{
  const auto shipDbId = message.getShipDbId();

  const auto maybeShip = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeShip)
  {
    warn("Failed to process jump cancelled message for ship " + bsgo::str(shipDbId));
    return;
  }

  auto ship         = m_coordinator->getEntity(*maybeShip);
  const auto status = ship.statusComp().status();

  const auto newStatus = bsgo::updateStatusAfterJumpCancellation(status);
  ship.statusComp().setStatus(newStatus);
}

void ShipMessageConsumer::handleTargetAcquired(const bsgo::TargetMessage &message) const
{
  const auto shipDbId   = message.getShipDbId();
  const auto targetKind = message.getTargetKind();
  const auto targetDbId = message.getTargetDbId();

  debug("handling target for " + bsgo::str(shipDbId) + ": " + bsgo::str(*targetDbId));

  const auto maybeShip = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeShip)
  {
    warn("Failed to process target message for ship " + bsgo::str(shipDbId));
    return;
  }

  auto ship        = m_coordinator->getEntity(*maybeShip);
  auto &targetComp = ship.targetComp();

  if (targetDbId)
  {
    const auto targetEntityId = determineTargetEntityId(*targetDbId, *targetKind);
    targetComp.setTarget(targetEntityId);
  }
  else
  {
    targetComp.clearTarget();
  }
}

auto ShipMessageConsumer::determineTargetEntityId(const bsgo::Uuid targetDbId,
                                                  const bsgo::EntityKind &kind) const -> bsgo::Uuid
{
  std::optional<bsgo::Uuid> entityId{};

  switch (kind)
  {
    case bsgo::EntityKind::ASTEROID:
      entityId = m_entityMapper.tryGetAsteroidEntityId(targetDbId);
      break;
    case bsgo::EntityKind::SHIP:
      entityId = m_entityMapper.tryGetShipEntityId(targetDbId);
      break;
    case bsgo::EntityKind::OUTPOST:
      entityId = m_entityMapper.tryGetOutpostEntityId(targetDbId);
      break;
    default:
      error("Failed to determine entity id for target " + bsgo::str(targetDbId),
            "Unsupported kind " + bsgo::str(kind));
      break;
  }

  if (!entityId)
  {
    error("Failed to determine entity id for target " + bsgo::str(targetDbId),
          "No entity attached to it");
  }

  return *entityId;
}

} // namespace pge
