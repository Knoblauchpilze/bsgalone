
#include "DockMessageConsumer.hh"
#include "EntityAddedMessage.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"

namespace bsgo {

DockMessageConsumer::DockMessageConsumer(const Services &services,
                                         IMessageQueue *const systemMessageQueue,
                                         IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("dock", {MessageType::DOCK})
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
  const auto &dockMessage = message.as<DockMessage>();

  switch (dockMessage.getTransition())
  {
    case DockTransition::DOCK:
      handleDocking(dockMessage);
      break;
    case DockTransition::UNDOCK:
      handleUndocking(dockMessage);
      break;
    case DockTransition::BACK_TO_OUTPOST:
      handleReturnToOutpost(dockMessage);
      break;
    default:
      error("Unexpected docking transition " + str(dockMessage.getTransition()));
  }
}

void DockMessageConsumer::handleDocking(const DockMessage &message) const
{
  const auto shipDbId = message.getShipDbId();

  if (!m_shipService->tryDock(shipDbId))
  {
    warn("Failed to process dock message for ship " + str(shipDbId));
    return;
  }

  auto out = std::make_unique<DockMessage>(shipDbId, DockTransition::DOCK);
  out->copyClientIdIfDefined(message);
  m_outputMessageQueue->pushMessage(std::move(out));

  publishLoadingMessages(LoadingTransition::DOCK, shipDbId, message);
}

void DockMessageConsumer::handleUndocking(const DockMessage &message) const
{
  const auto shipDbId   = message.getShipDbId();
  const auto systemDbId = m_shipService->getSystemDbIdForShip(shipDbId);

  auto out = std::make_unique<DockMessage>(shipDbId, DockTransition::UNDOCK);
  out->copyClientIdIfDefined(message);
  m_outputMessageQueue->pushMessage(std::move(out));

  auto added = std::make_unique<EntityAddedMessage>(systemDbId);
  PlayerShipData data{.dbId = shipDbId};
  added->setShipData(data);
  m_systemMessageQueue->pushMessage(std::move(added));

  publishLoadingMessages(LoadingTransition::UNDOCK, shipDbId, message);
}

void DockMessageConsumer::handleReturnToOutpost(const DockMessage &message) const
{
  const auto shipDbId = message.getShipDbId();

  if (!m_shipService->tryReturnToOutpost(shipDbId))
  {
    warn("Failed to process dock message for ship " + str(shipDbId));
    return;
  }

  publishLoadingMessages(LoadingTransition::DOCK, shipDbId, message);
}

void DockMessageConsumer::publishLoadingMessages(const LoadingTransition transition,
                                                 const Uuid shipDbId,
                                                 const DockMessage &originalDockMessage) const
{
  const auto playerDbId = m_shipService->getPlayerDbIdForShip(shipDbId);
  const auto systemDbId = m_shipService->getSystemDbIdForShip(shipDbId);

  auto started = std::make_unique<LoadingStartedMessage>(transition, playerDbId);
  started->setSystemDbId(systemDbId);
  started->copyClientIdIfDefined(originalDockMessage);
  m_systemMessageQueue->pushMessage(std::move(started));

  auto finished = std::make_unique<LoadingFinishedMessage>(transition, playerDbId);
  finished->setSystemDbId(systemDbId);
  finished->copyClientIdIfDefined(originalDockMessage);
  m_systemMessageQueue->pushMessage(std::move(finished));
}

} // namespace bsgo
