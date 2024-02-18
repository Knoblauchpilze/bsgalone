
#include "ShipMessageConsumer.hh"

namespace pge {

ShipMessageConsumer::ShipMessageConsumer(const bsgo::DatabaseEntityMapper &entityMapper,
                                         bsgo::CoordinatorShPtr coordinator)
  : bsgo::AbstractMessageConsumer("ship",
                                  {bsgo::MessageType::COMPONENT_SYNC,
                                   bsgo::MessageType::JUMP_REQUESTED,
                                   bsgo::MessageType::JUMP_CANCELLED,
                                   bsgo::MessageType::TARGET})
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

void ShipMessageConsumer::onMessageReceived(const bsgo::IMessage &message)
{
  switch (message.type())
  {
    case bsgo::MessageType::COMPONENT_SYNC:
      handleComponentSync(message.as<bsgo::ComponentSyncMessage>());
      break;
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

void ShipMessageConsumer::handleComponentSync(const bsgo::ComponentSyncMessage &message) const
{
  if (!m_entityMapper.doesPlayerHaveAnEntity())
  {
    // Most probably the player did not undock yet.
    return;
  }

  const auto entityKind = message.getEntityKind();
  switch (entityKind)
  {
    case bsgo::EntityKind::SHIP:
      handleShipComponentsSync(message);
      break;
    case bsgo::EntityKind::ASTEROID:
      handleAsteroidComponentsSync(message);
      break;
    default:
      error("Unsupported entity kind " + bsgo::str(entityKind) + " in component sync message");
      break;
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

void ShipMessageConsumer::handleShipComponentsSync(const bsgo::ComponentSyncMessage &message) const
{
  const auto shipDbId = message.getEntityDbId();

  const auto maybeShip = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeShip)
  {
    warn("Failed to process component sync message for ship " + bsgo::str(shipDbId));
    return;
  }

  /// TODO: Probably ignore modification for the player's ship.
  auto ship = m_coordinator->getEntity(*maybeShip);

  const auto maybeShipStatus = message.tryGetStatus();
  if (maybeShipStatus)
  {
    ship.statusComp().setStatus(*maybeShipStatus);
    info("Setting status of " + bsgo::str(shipDbId) + " to " + bsgo::str(*maybeShipStatus));
  }

  const auto maybeShipPosition = message.tryGetPosition();
  if (maybeShipPosition)
  {
    ship.transformComp().overridePosition(*maybeShipPosition);
  }

  const auto maybeShipSpeed = message.tryGetSpeed();
  if (maybeShipSpeed)
  {
    ship.velocityComp().overrideSpeed(*maybeShipSpeed);
  }

  const auto maybeShipAcceleration = message.tryGetAcceleration();
  if (maybeShipAcceleration)
  {
    ship.velocityComp().overrideAcceleration(*maybeShipAcceleration);
  }

  const auto maybeHealth = message.tryGetHealth();
  if (maybeHealth)
  {
    ship.healthComp().overrideValue(*maybeHealth);
  }

  const auto maybePower = message.tryGetPower();
  if (maybePower)
  {
    ship.powerComp().overrideValue(*maybePower);
  }
}

void ShipMessageConsumer::handleAsteroidComponentsSync(const bsgo::ComponentSyncMessage &message) const
{
  const auto asteroidDbId = message.getEntityDbId();

  const auto maybeAsteroid = m_entityMapper.tryGetAsteroidEntityId(asteroidDbId);
  if (!maybeAsteroid)
  {
    warn("Failed to process component sync message for asteroid " + bsgo::str(asteroidDbId));
    return;
  }

  auto asteroid = m_coordinator->getEntity(*maybeAsteroid);

  const auto maybeHealth = message.tryGetHealth();
  if (maybeHealth)
  {
    asteroid.healthComp().overrideValue(*maybeHealth);
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
