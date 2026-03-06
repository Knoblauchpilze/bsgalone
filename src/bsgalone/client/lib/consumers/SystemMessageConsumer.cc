
#include "SystemMessageConsumer.hh"
#include "AsteroidDataSource.hh"
#include "MessageUtils.hh"
#include "PlayerDataSource.hh"
#include "ShipDataSource.hh"

namespace bsgalone::client {

SystemMessageConsumer::SystemMessageConsumer(core::DatabaseEntityMapper &entityMapper,
                                             core::CoordinatorShPtr coordinator)
  : AbstractMessageConsumer("system",
                            {core::MessageType::SCANNED,
                             core::MessageType::ENTITY_ADDED,
                             core::MessageType::ENTITY_REMOVED})
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{
  if (nullptr == m_coordinator)
  {
    throw std::invalid_argument("Expected non null coordinator");
  }
}

void SystemMessageConsumer::onEventReceived(const core::IMessage &message)
{
  switch (message.type())
  {
    case core::MessageType::SCANNED:
      handleScanOperation(message.as<core::ScannedMessage>());
      break;
    case core::MessageType::ENTITY_ADDED:
      handleEntityAdded(message.as<core::EntityAddedMessage>());
      break;
    case core::MessageType::ENTITY_REMOVED:
      handleEntityRemoved(message.as<core::EntityRemovedMessage>());
      break;
    default:
      error("Unsupported message type " + str(message.type()));
  }
}

void SystemMessageConsumer::handleScanOperation(const core::ScannedMessage &message) const
{
  const auto asteroidDbId = message.getAsteroidDbId();

  const auto maybeAsteroid = m_entityMapper.tryGetAsteroidEntityId(asteroidDbId);
  if (!maybeAsteroid)
  {
    warn("Failed to process scan message for asteroid " + core::str(asteroidDbId));
    return;
  }

  auto asteroid = m_coordinator->getEntity(*maybeAsteroid);
  asteroid.scannedComp().scan();
}

void SystemMessageConsumer::handleEntityAdded(const core::EntityAddedMessage &message) const
{
  if (!m_entityMapper.doesPlayerHaveAnEntity())
  {
    // Most probably the player did not undock yet.
    return;
  }

  std::optional<core::Uuid> entityId{};

  const auto entityKind = message.getEntityKind();

  switch (entityKind)
  {
    case core::EntityKind::SHIP:
      handleShipCreation(*message.tryGetShipData());
      break;
    case core::EntityKind::ASTEROID:
      handleAsteroidCreation(*message.tryGetAsteroidData());
      break;
    case core::EntityKind::PLAYER:
      handlePlayerCreation(*message.tryGetPlayerData());
      break;
    default:
      error("Failed to handle creation of entity", "Unsupported kind " + core::str(entityKind));
      break;
  }
}

void SystemMessageConsumer::handleEntityRemoved(const core::EntityRemovedMessage &message) const
{
  if (!m_entityMapper.doesPlayerHaveAnEntity())
  {
    // Most probably the player did not undock yet.
    return;
  }

  const auto entityDbId = message.getEntityDbId();
  const auto entityKind = message.getEntityKind();

  const auto entityId = m_entityMapper.tryGetEntityId(entityDbId, entityKind);

  switch (entityKind)
  {
    case core::EntityKind::SHIP:
      m_entityMapper.tryRemoveEntityForShip(entityDbId);
      break;
    case core::EntityKind::ASTEROID:
      m_entityMapper.removeEntityForAsteroid(entityDbId);
      break;
    case core::EntityKind::PLAYER:
      m_entityMapper.removeEntityForPlayer(entityDbId);
    default:
      break;
  }

  if (!entityId)
  {
    error("Failed to handle removal of entity " + core::str(entityDbId),
          "Unsupported kind " + core::str(entityKind));
  }

  if (didPlayerShipDie(message, m_entityMapper))
  {
    // Do not remove the player ship entity if it dies, we disable everything
    // else anyway. This will be cleared when going back to the outpost.
    // However we set its status to DEAD to disable among other things firing
    // weapons or reloading of computers.
    auto playerShip = m_coordinator->getEntity(*entityId);
    playerShip.statusComp().setStatus(core::Status::DEAD);

    return;
  }
  if (didPlayerDie(message, m_entityMapper))
  {
    // Also do not remove the player's entity if it dies. As explained above
    // everything is disabled but we need the player to display its name.
    return;
  }

  const auto ent = m_coordinator->getEntity(*entityId);
  info("Deleting entity " + ent.str());
  m_coordinator->deleteEntity(*entityId);
}

void SystemMessageConsumer::handleShipCreation(const core::PlayerShipData &data) const
{
  core::ShipDataSource source;
  source.registerShip(*m_coordinator, data, m_entityMapper, false);

  info("Registered entity for ship " + core::str(data.dbId));
}

void SystemMessageConsumer::handleAsteroidCreation(const core::AsteroidData &data) const
{
  core::AsteroidDataSource source;
  source.registerAsteroid(*m_coordinator, data, m_entityMapper);

  info("Registered entity for asteroid " + core::str(data.dbId));
}

void SystemMessageConsumer::handlePlayerCreation(const core::PlayerData &data) const
{
  core::PlayerDataSource source;
  source.registerPlayer(*m_coordinator, data, m_entityMapper);

  info("Registered entity for player " + core::str(data.dbId));
}

} // namespace bsgalone::client
