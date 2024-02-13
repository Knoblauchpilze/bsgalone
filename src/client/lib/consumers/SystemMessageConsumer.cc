
#include "SystemMessageConsumer.hh"
#include "MessageUtils.hh"

namespace pge {

SystemMessageConsumer::SystemMessageConsumer(bsgo::DatabaseEntityMapper &entityMapper,
                                             bsgo::CoordinatorShPtr coordinator)
  : bsgo::AbstractMessageConsumer("system",
                                  {bsgo::MessageType::SCANNED,
                                   bsgo::MessageType::ENTITY_ADDED,
                                   bsgo::MessageType::ENTITY_REMOVED})
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

void SystemMessageConsumer::onMessageReceived(const bsgo::IMessage &message)
{
  switch (message.type())
  {
    case bsgo::MessageType::SCANNED:
      handleScanOperation(message.as<bsgo::ScannedMessage>());
      break;
    case bsgo::MessageType::ENTITY_ADDED:
      handleEntityAdded(message.as<bsgo::EntityAddedMessage>());
      break;
    case bsgo::MessageType::ENTITY_REMOVED:
      handleEntityRemoved(message.as<bsgo::EntityRemovedMessage>());
      break;
    default:
      error("Unsupported message type " + bsgo::str(message.type()));
  }
}

void SystemMessageConsumer::handleScanOperation(const bsgo::ScannedMessage &message) const
{
  const auto asteroidDbId = message.getAsteroidDbId();

  const auto maybeAsteroid = m_entityMapper.tryGetAsteroidEntityId(asteroidDbId);
  if (!maybeAsteroid)
  {
    warn("Failed to process scan message for asteroid " + bsgo::str(asteroidDbId));
    return;
  }

  auto asteroid = m_coordinator->getEntity(*maybeAsteroid);
  asteroid.scannedComp().scan();
}

void SystemMessageConsumer::handleEntityAdded(const bsgo::EntityAddedMessage &message) const
{
  std::optional<bsgo::Uuid> entityId{};

  const auto entityKind = message.getEntityKind();
  const auto entityDbId = message.getEntityDbId();

  switch (entityKind)
  {
    case bsgo::EntityKind::SHIP:
      handleShipCreation(entityDbId);
      break;
    case bsgo::EntityKind::ASTEROID:
      handleAsteroidCreation(entityDbId);
      break;
    default:
      error("Failed to handle creation of entity " + bsgo::str(entityDbId),
            "Unsupported kind " + bsgo::str(entityKind));
      break;
  }
}

void SystemMessageConsumer::handleEntityRemoved(const bsgo::EntityRemovedMessage &message) const
{
  std::optional<bsgo::Uuid> entityId{};

  const auto entityKind = message.getEntityKind();
  const auto entityDbId = message.getEntityDbId();

  switch (entityKind)
  {
    case bsgo::EntityKind::SHIP:
      entityId = m_entityMapper.tryGetShipEntityId(entityDbId);
      m_entityMapper.tryRemoveEntityForShip(entityDbId);
      break;
    case bsgo::EntityKind::ASTEROID:
      entityId = m_entityMapper.tryGetAsteroidEntityId(entityDbId);
      m_entityMapper.removeEntityForAsteroid(entityDbId);
      break;
    default:
      break;
  }

  if (!entityId)
  {
    error("Failed to handle removal of entity " + bsgo::str(entityDbId),
          "Unsupported kind " + bsgo::str(entityKind));
  }

  if (didPlayerShipDied(message, m_entityMapper))
  {
    // Do not remove the player ship entity if it dies, we disable everything
    // else anyway. This will be cleared when going back to the outpost.
    return;
  }

  const auto ent = m_coordinator->getEntity(*entityId);
  info("Deleting entity " + ent.str());
  m_coordinator->deleteEntity(*entityId);
}

void SystemMessageConsumer::handleShipCreation(const bsgo::Uuid shipDbId) const
{
  const auto playerShipDbId = m_entityMapper.tryGetPlayerShipDbId();
  if (!playerShipDbId)
  {
    error("Failed to process ship entity added for " + bsgo::str(shipDbId),
          "Player ship is not defined");
  }
  if (*playerShipDbId == shipDbId)
  {
    debug("Ignoring creation of ship " + bsgo::str(shipDbId) + " as it is the player's ship");
    return;
  }

  /// TODO: Handle ship creation in the client
  warn("Should handle ship " + bsgo::str(shipDbId) + " creation");
}

void SystemMessageConsumer::handleAsteroidCreation(const bsgo::Uuid asteroidDbId) const
{
  /// TODO: Handle asteroid creation in the client
  warn("Should handle asteroid " + bsgo::str(asteroidDbId) + " creation");
}

} // namespace pge
