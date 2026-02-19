
#include "DockMessageConsumer.hh"
#include "EntityAddedMessage.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"

namespace bsgo {

DockMessageConsumer::DockMessageConsumer(const Services &services,
                                         bsgalone::core::IMessageQueue *const systemMessageQueue,
                                         bsgalone::core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("dock", {bsgalone::core::MessageType::DOCK})
  , m_shipService(services.ship)
  , m_entityService(services.entity)
  , m_systemMessageQueue(systemMessageQueue)
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_shipService)
  {
    throw std::invalid_argument("Expected non null ship service");
  }
  if (nullptr == m_entityService)
  {
    throw std::invalid_argument("Expected non null entity service");
  }
  if (nullptr == m_systemMessageQueue)
  {
    throw std::invalid_argument("Expected non null system message queue");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void DockMessageConsumer::onMessageReceived(const bsgalone::core::IMessage &message)
{
  const auto &dockMessage = message.as<bsgalone::core::DockMessage>();

  switch (dockMessage.getTransition())
  {
    case bsgalone::core::DockTransition::DOCK:
      handleDocking(dockMessage);
      break;
    case bsgalone::core::DockTransition::UNDOCK:
      handleUndocking(dockMessage);
      break;
    case bsgalone::core::DockTransition::BACK_TO_OUTPOST:
      handleReturnToOutpost(dockMessage);
      break;
    default:
      error("Unexpected docking transition " + str(dockMessage.getTransition()));
  }
}

void DockMessageConsumer::handleDocking(const bsgalone::core::DockMessage &message) const
{
  const auto shipDbId = message.getShipDbId();

  if (!m_shipService->tryDock(shipDbId))
  {
    warn("Failed to process dock message for ship " + str(shipDbId));
    return;
  }

  m_outputMessageQueue->pushMessage(message.clone());
  publishLoadingMessages(LoadingTransition::DOCK, shipDbId);
}

void DockMessageConsumer::handleUndocking(const bsgalone::core::DockMessage &message) const
{
  const auto shipDbId   = message.getShipDbId();
  const auto systemDbId = m_shipService->getSystemDbIdForShip(shipDbId);

  m_outputMessageQueue->pushMessage(message.clone());

  auto added = std::make_unique<EntityAddedMessage>(systemDbId);
  PlayerShipData data{.dbId = shipDbId};
  added->setShipData(data);
  m_systemMessageQueue->pushMessage(std::move(added));

  publishLoadingMessages(LoadingTransition::UNDOCK, shipDbId);
}

void DockMessageConsumer::handleReturnToOutpost(const bsgalone::core::DockMessage &message) const
{
  const auto shipDbId = message.getShipDbId();

  if (!m_shipService->tryReturnToOutpost(shipDbId))
  {
    warn("Failed to process dock message for ship " + str(shipDbId));
    return;
  }

  publishLoadingMessages(LoadingTransition::DOCK, shipDbId);
}

void DockMessageConsumer::publishLoadingMessages(const LoadingTransition transition,
                                                 const Uuid shipDbId) const
{
  const auto playerDbId = m_shipService->getPlayerDbIdForShip(shipDbId);
  const auto systemDbId = m_shipService->getSystemDbIdForShip(shipDbId);

  auto started = std::make_unique<LoadingStartedMessage>(transition, playerDbId);
  started->setSystemDbId(systemDbId);
  m_systemMessageQueue->pushMessage(std::move(started));

  auto finished = std::make_unique<LoadingFinishedMessage>(transition, playerDbId);
  finished->setSystemDbId(systemDbId);
  m_systemMessageQueue->pushMessage(std::move(finished));
}

} // namespace bsgo
