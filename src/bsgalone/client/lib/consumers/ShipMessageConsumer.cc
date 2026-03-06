
#include "ShipMessageConsumer.hh"
#include "VectorUtils.hh"

namespace bsgalone::client {

ShipMessageConsumer::ShipMessageConsumer(const core::DatabaseEntityMapper &entityMapper,
                                         core::CoordinatorShPtr coordinator)
  : AbstractMessageConsumer("ship",
                            {core::MessageType::COMPONENT_SYNC,
                             core::MessageType::JUMP_REQUESTED,
                             core::MessageType::JUMP_CANCELLED,
                             core::MessageType::TARGET})
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

void ShipMessageConsumer::onEventReceived(const core::IMessage &message)
{
  switch (message.type())
  {
    case core::MessageType::COMPONENT_SYNC:
      handleComponentSync(message.as<core::ComponentSyncMessage>());
      break;
    case core::MessageType::JUMP_REQUESTED:
      handleJumpRequested(message.as<core::JumpRequestedMessage>());
      break;
    case core::MessageType::JUMP_CANCELLED:
      handleJumpCancelled(message.as<core::JumpCancelledMessage>());
      break;
    case core::MessageType::TARGET:
      handleTargetAcquired(message.as<core::TargetMessage>());
      break;
    default:
      error("Unsupported message type " + str(message.type()));
  }
}

void ShipMessageConsumer::handleComponentSync(const core::ComponentSyncMessage &message) const
{
  if (!m_entityMapper.doesPlayerHaveAnEntity())
  {
    // Most probably the player did not undock yet.
    return;
  }

  const auto entityKind = message.getEntityKind();
  switch (entityKind)
  {
    case core::EntityKind::SHIP:
      handleShipComponentsSync(message);
      break;
    case core::EntityKind::ASTEROID:
      handleAsteroidComponentsSync(message);
      break;
    case core::EntityKind::OUTPOST:
      handleOutpostComponentsSync(message);
      break;
    default:
      error("Unsupported entity kind " + core::str(entityKind) + " in component sync message");
      break;
  }
}

void ShipMessageConsumer::handleJumpRequested(const core::JumpRequestedMessage &message) const
{
  const auto shipDbId = message.getShipDbId();

  const auto maybeShip = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeShip)
  {
    warn("Failed to process jump requested message for ship " + core::str(shipDbId));
    return;
  }

  auto ship         = m_coordinator->getEntity(*maybeShip);
  const auto status = ship.statusComp().status();

  const auto newStatus = core::updateStatusForJump(status);
  ship.statusComp().setStatus(newStatus);
}

void ShipMessageConsumer::handleJumpCancelled(const core::JumpCancelledMessage &message) const
{
  const auto shipDbId = message.getShipDbId();

  const auto maybeShip = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeShip)
  {
    warn("Failed to process jump cancelled message for ship " + core::str(shipDbId));
    return;
  }

  auto ship         = m_coordinator->getEntity(*maybeShip);
  const auto status = ship.statusComp().status();

  const auto newStatus = core::updateStatusAfterJumpCancellation(status);
  ship.statusComp().setStatus(newStatus);
}

void ShipMessageConsumer::handleTargetAcquired(const core::TargetMessage &message) const
{
  if (!m_entityMapper.doesPlayerHaveAnEntity())
  {
    // Most probably the player did not undock yet: even if the update might not be
    // related to the player ship, when there's no player ship the logic of the client
    // is to not have any entity loaded so this update will fail.
    return;
  }

  const auto maybeSourceEntityId = m_entityMapper.tryGetEntityId(message.getSourceDbId(),
                                                                 message.getSourceKind());
  if (!maybeSourceEntityId)
  {
    error("Failed to process target updated for " + core::str(message.getSourceDbId()),
          "No entity attached to source");
  }

  auto entity      = m_coordinator->getEntity(*maybeSourceEntityId);
  auto &targetComp = entity.targetComp();

  const auto maybeTargetDbId = message.tryGetTargetDbId();
  const auto maybeTargetKind = message.tryGetTargetKind();
  if (maybeTargetDbId && maybeTargetKind)
  {
    const auto maybeTargetEntityId = m_entityMapper.tryGetEntityId(*maybeTargetDbId,
                                                                   *maybeTargetKind);
    if (!maybeTargetEntityId)
    {
      error("Failed to process target updated for " + core::str(message.getSourceDbId()),
            "No entity attached to target");
    }
    targetComp.setTarget(*maybeTargetEntityId);
  }
  else
  {
    targetComp.clearTarget();
  }
}

namespace {
constexpr auto DISTANCE_THRESHOLD_FOR_FORCED_UPDATE = 1.0f;

bool positionDifferenceIsBiggerThanThreshold(const core::Entity &ship,
                                             const Eigen::Vector3f &newPosition)
{
  const Eigen::Vector3f currentPosition = ship.transformComp().position();

  const float delta = (newPosition - currentPosition).norm();
  return delta >= DISTANCE_THRESHOLD_FOR_FORCED_UPDATE;
}
} // namespace

void ShipMessageConsumer::handleShipComponentsSync(const core::ComponentSyncMessage &message) const
{
  const auto shipDbId = message.getEntityDbId();

  const auto maybeShip = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeShip)
  {
    warn("Failed to process component sync message for ship " + core::str(shipDbId));
    return;
  }

  auto ship = m_coordinator->getEntity(*maybeShip);

  const auto maybeShipStatus = message.tryGetStatus();
  if (maybeShipStatus)
  {
    const auto oldStatus = ship.statusComp().status();
    ship.statusComp().setStatus(*maybeShipStatus);

    if (oldStatus != *maybeShipStatus)
    {
      info("Setting status of " + core::str(shipDbId) + " to " + core::str(*maybeShipStatus));
    }
  }

  const auto maybeShipPosition = message.tryGetPosition();
  if (maybeShipPosition)
  {
    const auto maybePlayerShipDbId = m_entityMapper.tryGetPlayerShipDbId();
    const auto isPlayerShipUpdate  = maybePlayerShipDbId && *maybePlayerShipDbId == shipDbId;
    if (!isPlayerShipUpdate || positionDifferenceIsBiggerThanThreshold(ship, *maybeShipPosition))
    {
      if (isPlayerShipUpdate)
      {
        warn("Overriding player ship position with " + core::str(*maybeShipPosition)
             + " as it's too different from local position "
             + core::str(ship.transformComp().position()));
      }

      verbose("override position from " + core::str(ship.transformComp().position()) + " with "
              + core::str(*maybeShipPosition));

      ship.transformComp().overridePosition(*maybeShipPosition);
    }
  }

  const auto maybeShipSpeed = message.tryGetSpeed();
  if (maybeShipSpeed)
  {
    verbose("override speed from " + core::str(ship.velocityComp().speed()) + " with "
            + core::str(*maybeShipSpeed));

    ship.velocityComp().overrideSpeed(*maybeShipSpeed);
  }

  const auto maybeShipAcceleration = message.tryGetAcceleration();
  if (maybeShipAcceleration)
  {
    verbose("override acceleration from " + core::str(ship.velocityComp().acceleration()) + " with "
            + core::str(*maybeShipAcceleration));
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

void ShipMessageConsumer::handleAsteroidComponentsSync(const core::ComponentSyncMessage &message) const
{
  const auto asteroidDbId = message.getEntityDbId();

  const auto maybeAsteroid = m_entityMapper.tryGetAsteroidEntityId(asteroidDbId);
  if (!maybeAsteroid)
  {
    warn("Failed to process component sync message for asteroid " + core::str(asteroidDbId));
    return;
  }

  auto asteroid = m_coordinator->getEntity(*maybeAsteroid);

  const auto maybeHealth = message.tryGetHealth();
  if (maybeHealth)
  {
    asteroid.healthComp().overrideValue(*maybeHealth);
  }
}

void ShipMessageConsumer::handleOutpostComponentsSync(const core::ComponentSyncMessage &message) const
{
  const auto outpostDbId = message.getEntityDbId();

  const auto maybeOutpost = m_entityMapper.tryGetOutpostEntityId(outpostDbId);
  if (!maybeOutpost)
  {
    warn("Failed to process component sync message for outpost " + core::str(outpostDbId));
    return;
  }

  auto outpost = m_coordinator->getEntity(*maybeOutpost);

  const auto maybeHealth = message.tryGetHealth();
  if (maybeHealth)
  {
    outpost.healthComp().overrideValue(*maybeHealth);
  }

  const auto maybePower = message.tryGetPower();
  if (maybePower)
  {
    outpost.powerComp().overrideValue(*maybePower);
  }
}

} // namespace bsgalone::client
