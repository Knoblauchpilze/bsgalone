
#include "DockMessageConsumer.hh"
#include "DockMessage.hh"

namespace bsgo {

DockMessageConsumer::DockMessageConsumer(const Services &services, IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("dock", {bsgo::MessageType::DOCK})
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
  const auto &dockMessage = message.as<bsgo::DockMessage>();

  if (dockMessage.isDocking() && DockState::STARTED == dockMessage.getDockState())
  {
    handleDocking(dockMessage.getShipDbId(), dockMessage.getShipEntityId());
  }
}

void DockMessageConsumer::handleDocking(const Uuid &shipDbId, const Uuid &shipEntityId) const
{
  if (!m_shipService->tryDock(shipEntityId))
  {
    warn("Failed to process dock message for ship " + str(shipDbId));
    return;
  }

  m_messageQueue->pushMessage(
    std::make_unique<bsgo::DockMessage>(shipDbId, shipEntityId, true, DockState::COMPLETED));
}

} // namespace bsgo
