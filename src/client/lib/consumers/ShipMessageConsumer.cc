
#include "ShipMessageConsumer.hh"
#include "VectorUtils.hh"

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
    case bsgo::EntityKind::OUTPOST:
      handleOutpostComponentsSync(message);
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

namespace {
auto tryDetermineEntityId(const bsgo::DatabaseEntityMapper &entityMapper,
                          const bsgo::Uuid targetDbId,
                          const bsgo::EntityKind &kind) -> std::optional<bsgo::Uuid>
{
  std::optional<bsgo::Uuid> entityId{};

  switch (kind)
  {
    case bsgo::EntityKind::ASTEROID:
      return entityMapper.tryGetAsteroidEntityId(targetDbId);
    case bsgo::EntityKind::SHIP:
      return entityMapper.tryGetShipEntityId(targetDbId);
    case bsgo::EntityKind::OUTPOST:
      return entityMapper.tryGetOutpostEntityId(targetDbId);
    default:
      throw std::invalid_argument("Unsupported kind " + bsgo::str(kind) + " to determine entity");
  }
}

} // namespace

void ShipMessageConsumer::handleTargetAcquired(const bsgo::TargetMessage &message) const
{
  const auto maybeSourceEntityId = tryDetermineEntityId(m_entityMapper,
                                                        message.getSourceDbId(),
                                                        message.getSourceKind());
  if (!maybeSourceEntityId)
  {
    error("Failed to process target updated for " + bsgo::str(message.getSourceDbId()),
          "No entity attached to source");
  }

  auto entity      = m_coordinator->getEntity(*maybeSourceEntityId);
  auto &targetComp = entity.targetComp();

  const auto maybeTargetDbId = message.tryGetTargetDbId();
  const auto maybeTargetKind = message.tryGetTargetKind();
  if (maybeTargetDbId && maybeTargetKind)
  {
    const auto maybeTargetEntityId = tryDetermineEntityId(m_entityMapper,
                                                          *maybeTargetDbId,
                                                          *maybeTargetKind);
    if (!maybeTargetEntityId)
    {
      error("Failed to process target updated for " + bsgo::str(message.getSourceDbId()),
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

bool positionDifferenceIsBiggerThanThreshold(const bsgo::Entity &ship,
                                             const Eigen::Vector3f &newPosition)
{
  const Eigen::Vector3f currentPosition = ship.transformComp().position();

  const float delta = (newPosition - currentPosition).norm();
  return delta >= DISTANCE_THRESHOLD_FOR_FORCED_UPDATE;
}
} // namespace

void ShipMessageConsumer::handleShipComponentsSync(const bsgo::ComponentSyncMessage &message) const
{
  const auto shipDbId = message.getEntityDbId();

  const auto maybeShip = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeShip)
  {
    warn("Failed to process component sync message for ship " + bsgo::str(shipDbId));
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
      info("Setting status of " + bsgo::str(shipDbId) + " to " + bsgo::str(*maybeShipStatus));
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
        warn("Overriding player ship position with " + bsgo::str(*maybeShipPosition)
             + " as it's too different from local position "
             + bsgo::str(ship.transformComp().position()));
      }

      verbose("override position from " + bsgo::str(ship.transformComp().position()) + " with "
              + bsgo::str(*maybeShipPosition));

      ship.transformComp().overridePosition(*maybeShipPosition);
    }
  }

  const auto maybeShipSpeed = message.tryGetSpeed();
  if (maybeShipSpeed)
  {
    verbose("override speed from " + bsgo::str(ship.velocityComp().speed()) + " with "
            + bsgo::str(*maybeShipSpeed));

    ship.velocityComp().overrideSpeed(*maybeShipSpeed);
  }

  const auto maybeShipAcceleration = message.tryGetAcceleration();
  if (maybeShipAcceleration)
  {
    verbose("override acceleration from " + bsgo::str(ship.velocityComp().acceleration()) + " with "
            + bsgo::str(*maybeShipAcceleration));
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

void ShipMessageConsumer::handleOutpostComponentsSync(const bsgo::ComponentSyncMessage &message) const
{
  const auto outpostDbId = message.getEntityDbId();

  const auto maybeOutpost = m_entityMapper.tryGetOutpostEntityId(outpostDbId);
  if (!maybeOutpost)
  {
    warn("Failed to process component sync message for outpost " + bsgo::str(outpostDbId));
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

} // namespace pge
