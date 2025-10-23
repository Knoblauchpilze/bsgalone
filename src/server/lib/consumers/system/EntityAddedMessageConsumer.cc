
#include "EntityAddedMessageConsumer.hh"
#include "EntityAddedMessage.hh"

namespace bsgo {

EntityAddedMessageConsumer::EntityAddedMessageConsumer(const Services &services,
                                                       IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("entity", {MessageType::ENTITY_ADDED})
  , m_entityService(services.entity)
  , m_loadingService(services.loading)
  , m_outputMessageQueue(outputMessageQueue)
{
  addModule("added");

  if (nullptr == m_entityService)
  {
    throw std::invalid_argument("Expected non null entity service");
  }
  if (nullptr == m_loadingService)
  {
    throw std::invalid_argument("Expected non null loading service");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void EntityAddedMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &added = message.as<EntityAddedMessage>();

  const auto entityKind = added.getEntityKind();
  const auto systemDbId = added.getSystemDbId();

  switch (entityKind)
  {
    case EntityKind::SHIP:
      handleShipAdded(systemDbId, *added.tryGetShipData());
      break;
    case EntityKind::ASTEROID:
      handleAsteroidAdded(systemDbId, *added.tryGetAsteroidData());
      break;
    default:
      error("Unsupported type of entity added: " + str(entityKind));
  }
}

void EntityAddedMessageConsumer::handleShipAdded(const Uuid systemDbId,
                                                 const PlayerShipData &data) const
{
  if (!m_entityService->tryCreateShipEntity(data.dbId))
  {
    warn("Failed to process ship " + str(data.dbId) + " added in system " + str(systemDbId));
    return;
  }

  // The input ship data is not complete. We need to get all of it from the
  // loading service before sending the message to the client applications.
  const auto shipData = m_loadingService->getShipById(data.dbId);

  // In case the ship belongs to a player we also need to send a message to
  // add this player to the system.
  if (shipData.dbShip.player)
  {
    const auto playerData = m_loadingService->getPlayerById(*shipData.dbShip.player);

    auto playerAdded = std::make_unique<EntityAddedMessage>(systemDbId);
    PlayerData data{.dbId = playerData.id, .name = playerData.name};
    playerAdded->setPlayerData(data);
    m_outputMessageQueue->pushMessage(std::move(playerAdded));
  }

  auto shipAdded = std::make_unique<EntityAddedMessage>(systemDbId);
  shipAdded->setShipData(shipData.toPlayerShipData());
  m_outputMessageQueue->pushMessage(std::move(shipAdded));
}

void EntityAddedMessageConsumer::handleAsteroidAdded(const Uuid systemDbId,
                                                     const AsteroidData &data) const
{
  if (!m_entityService->tryCreateAsteroidEntity(data.dbId))
  {
    warn("Failed to process asteroid " + str(data.dbId) + " added in system " + str(systemDbId));
    return;
  }

  // The input asteroid data is not complete. We need to get all of it from
  // the loading service before sending the message to the client applications.
  const auto asteroidData = m_loadingService->getAsteroidById(data.dbId);

  auto asteroidAdded = std::make_unique<EntityAddedMessage>(systemDbId);
  asteroidAdded->setAsteroidData(asteroidData.toAsteroidData());
  m_outputMessageQueue->pushMessage(std::move(asteroidAdded));
}

} // namespace bsgo
