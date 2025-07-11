
#include "SystemMessageConsumer.hh"
#include "AsteroidDataSource.hh"
#include "MessageUtils.hh"
#include "PlayerDataSource.hh"
#include "ShipDataSource.hh"

namespace pge {

SystemMessageConsumer::SystemMessageConsumer(bsgo::DatabaseEntityMapper &entityMapper,
                                             bsgo::CoordinatorShPtr coordinator,
                                             bsgo::EntityServicePtr entityService)
  : bsgo::AbstractMessageConsumer("system",
                                  {bsgo::MessageType::SCANNED,
                                   bsgo::MessageType::ENTITY_ADDED,
                                   bsgo::MessageType::ENTITY_REMOVED})
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
  , m_entityService(std::move(entityService))
{
  if (nullptr == m_entityService)
  {
    throw std::invalid_argument("Expected non null entity service");
  }
}

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
  if (!m_entityMapper.doesPlayerHaveAnEntity())
  {
    // Most probably the player did not undock yet.
    return;
  }

  std::optional<bsgo::Uuid> entityId{};

  const auto entityKind = message.getEntityKind();

  info("received entity added message for " + bsgo::str(entityKind)
       + ", system: " + bsgo::str(message.getSystemDbId()));

  switch (entityKind)
  {
    case bsgo::EntityKind::SHIP:
      handleShipCreation(*message.tryGetShipData());
      break;
    case bsgo::EntityKind::ASTEROID:
      handleAsteroidCreation(*message.tryGetAsteroidData());
      break;
    case bsgo::EntityKind::PLAYER:
      handlePlayerCreation(*message.tryGetPlayerData());
      break;
    default:
      error("Failed to handle creation of entity", "Unsupported kind " + bsgo::str(entityKind));
      break;
  }
}

void SystemMessageConsumer::handleEntityRemoved(const bsgo::EntityRemovedMessage &message) const
{
  if (!m_entityMapper.doesPlayerHaveAnEntity())
  {
    // Most probably the player did not undock yet.
    return;
  }

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
    case bsgo::EntityKind::PLAYER:
      entityId = m_entityMapper.tryGetPlayerEntityId(entityDbId);
      m_entityMapper.removeEntityForPlayer(entityDbId);
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
    // However we set its status to DEAD to disable among other things firing
    // weapons or reloading of computers.
    auto playerShip = m_coordinator->getEntity(*entityId);
    playerShip.statusComp().setStatus(bsgo::Status::DEAD);

    return;
  }

  const auto ent = m_coordinator->getEntity(*entityId);
  info("Deleting entity " + ent.str());
  m_coordinator->deleteEntity(*entityId);
}

void SystemMessageConsumer::handleShipCreation(const bsgo::ShipData &data) const
{
  bsgo::ShipDataSource source;
  source.registerShip(*m_coordinator, data, m_entityMapper, false);

  info("Registered entity for ship " + bsgo::str(data.dbId));
}

void SystemMessageConsumer::handleAsteroidCreation(const bsgo::AsteroidData &data) const
{
  bsgo::AsteroidDataSource source;
  source.registerAsteroid(*m_coordinator, data, m_entityMapper);

  info("Registered entity for asteroid " + bsgo::str(data.dbId));
}

void SystemMessageConsumer::handlePlayerCreation(const bsgo::PlayerData &data) const
{
  bsgo::PlayerDataSource source;
  source.registerPlayer(*m_coordinator, data, m_entityMapper);

  info("Registered entity for player " + bsgo::str(data.dbId));
}

} // namespace pge
