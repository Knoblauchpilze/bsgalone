
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
    default:
      error("Unsupported type of entity added: " + str(entityKind));
  }
}

void EntityAddedMessageConsumer::handleShipAdded(const Uuid systemDbId, const ShipData &data) const
{
  if (!m_entityService->tryCreateShipEntity(data.dbId))
  {
    warn("Failed to process ship " + str(data.dbId) + " added in system " + str(systemDbId));
    return;
  }

  // The input ship data is not complete. We need to get all of it from the
  // loading service before sending the message to the client applications.
  const auto shipData = m_loadingService->getShipById(data.dbId);

  auto out = std::make_unique<EntityAddedMessage>(systemDbId);
  out->setShipData(shipData.toShipData());
  m_outputMessageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
