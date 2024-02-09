
#include "SystemMessageConsumer.hh"

namespace pge {

SystemMessageConsumer::SystemMessageConsumer(bsgo::DatabaseEntityMapper &entityMapper,
                                             bsgo::CoordinatorShPtr coordinator)
  : bsgo::AbstractMessageConsumer("system",
                                  {bsgo::MessageType::SCANNED, bsgo::MessageType::ENTITY_DIED})
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
    case bsgo::MessageType::ENTITY_DIED:
      handleEntityDeath(message.as<bsgo::EntityDiedMessage>());
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

void SystemMessageConsumer::handleEntityDeath(const bsgo::EntityDiedMessage &message) const
{
  std::optional<bsgo::Uuid> entityId{};

  const auto entityKind = message.getEntityKind();
  const auto entityDbId = message.getEntityDbId();

  switch (entityKind)
  {
    case bsgo::EntityKind::SHIP:
      entityId = m_entityMapper.tryGetShipEntityId(entityDbId);
      m_entityMapper.removeEntityForShip(entityDbId);
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
    error("Failed to handle death of entity " + bsgo::str(entityDbId),
          "Unsupported kind " + bsgo::str(entityKind));
  }

  const auto ent = m_coordinator->getEntity(*entityId);
  info("Deleting entity " + ent.str());
  m_coordinator->deleteEntity(*entityId);
}

} // namespace pge
