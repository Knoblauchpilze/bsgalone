
#include "DockMessageConsumer.hh"
#include "EntityAddedMessage.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"

namespace bsgalone::server {

DockMessageConsumer::DockMessageConsumer(const Services &services,
                                         core::IMessageQueue *const systemMessageQueue,
                                         core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("dock", {core::MessageType::DOCK})
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

void DockMessageConsumer::onEventReceived(const core::IMessage &message)
{
  const auto &dockMessage = message.as<core::DockMessage>();

  switch (dockMessage.getTransition())
  {
    case core::DockTransition::DOCK:
      handleDocking(dockMessage);
      break;
    case core::DockTransition::UNDOCK:
      handleUndocking(dockMessage);
      break;
    case core::DockTransition::BACK_TO_OUTPOST:
      handleReturnToOutpost(dockMessage);
      break;
    default:
      error("Unexpected docking transition " + str(dockMessage.getTransition()));
  }
}

void DockMessageConsumer::handleDocking(const core::DockMessage &message) const
{
  const auto shipDbId = message.getShipDbId();

  if (!m_shipService->tryDock(shipDbId))
  {
    warn("Failed to process dock message for ship " + core::str(shipDbId));
    return;
  }

  m_outputMessageQueue->pushEvent(message.clone());
  publishLoadingMessages(core::LoadingTransition::DOCK, shipDbId);
}

void DockMessageConsumer::handleUndocking(const core::DockMessage &message) const
{
  const auto shipDbId   = message.getShipDbId();
  const auto systemDbId = m_shipService->getSystemDbIdForShip(shipDbId);

  m_outputMessageQueue->pushEvent(message.clone());

  auto added = std::make_unique<core::EntityAddedMessage>(systemDbId);
  core::PlayerShipData data{.dbId = shipDbId};
  added->setShipData(data);
  m_systemMessageQueue->pushEvent(std::move(added));

  publishLoadingMessages(core::LoadingTransition::UNDOCK, shipDbId);
}

void DockMessageConsumer::handleReturnToOutpost(const core::DockMessage &message) const
{
  const auto shipDbId = message.getShipDbId();

  if (!m_shipService->tryReturnToOutpost(shipDbId))
  {
    warn("Failed to process dock message for ship " + core::str(shipDbId));
    return;
  }

  publishLoadingMessages(core::LoadingTransition::DOCK, shipDbId);
}

void DockMessageConsumer::publishLoadingMessages(const core::LoadingTransition transition,
                                                 const core::Uuid shipDbId) const
{
  const auto playerDbId = m_shipService->getPlayerDbIdForShip(shipDbId);
  const auto systemDbId = m_shipService->getSystemDbIdForShip(shipDbId);

  auto started = std::make_unique<core::LoadingStartedMessage>(transition, playerDbId);
  started->setSystemDbId(systemDbId);
  m_systemMessageQueue->pushEvent(std::move(started));

  auto finished = std::make_unique<core::LoadingFinishedMessage>(transition, playerDbId);
  finished->setSystemDbId(systemDbId);
  m_systemMessageQueue->pushEvent(std::move(finished));
}

} // namespace bsgalone::server
