
#include "DockMessageConsumer.hh"
#include "DockMessage.hh"

namespace bsgo {

DockMessageConsumer::DockMessageConsumer(const Services &services, IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("dock", {MessageType::DOCK})
  , m_shipService(services.ship)
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_shipService)
  {
    throw std::invalid_argument("Expected non null ship service");
  }
  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void DockMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &dockMessage = message.as<DockMessage>();

  if (dockMessage.isDocking() && !dockMessage.validated())
  {
    handleDocking(dockMessage.getShipDbId(), dockMessage.getShipEntityId());
  }
  if (!dockMessage.isDocking() && !dockMessage.validated())
  {
    handleUndocking(dockMessage.getShipDbId(), dockMessage.getShipEntityId());
  }
}

void DockMessageConsumer::handleDocking(const Uuid &shipDbId, const Uuid &shipEntityId) const
{
  if (!m_shipService->tryDock(shipDbId, shipEntityId))
  {
    warn("Failed to process dock message for ship " + str(shipDbId));
    return;
  }

  auto message = std::make_unique<DockMessage>(shipDbId, shipEntityId, true);
  message->validate(true);
  m_messageQueue->pushMessage(std::move(message));
}

void DockMessageConsumer::handleUndocking(const Uuid &shipDbId, const Uuid &shipEntityId) const
{
  if (!m_shipService->tryUndock(shipDbId, shipEntityId))
  {
    warn("Failed to process undock message for ship " + str(shipDbId));
    return;
  }

  auto message = std::make_unique<DockMessage>(shipDbId, shipEntityId, false);
  message->validate(true);
  m_messageQueue->pushMessage(std::move(message));
}

} // namespace bsgo
