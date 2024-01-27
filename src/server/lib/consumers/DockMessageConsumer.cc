
#include "DockMessageConsumer.hh"

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
    handleDocking(dockMessage);
  }
  if (!dockMessage.isDocking() && !dockMessage.validated())
  {
    handleUndocking(dockMessage);
  }
}

void DockMessageConsumer::handleDocking(const DockMessage &message) const
{
  const auto shipDbId     = message.getShipDbId();
  const auto shipEntityId = message.getShipEntityId();

  if (!m_shipService->tryDock(shipDbId, shipEntityId))
  {
    warn("Failed to process dock message for ship " + str(shipDbId));
    return;
  }

  auto out = std::make_unique<DockMessage>(shipDbId, shipEntityId, true);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_messageQueue->pushMessage(std::move(out));
}

void DockMessageConsumer::handleUndocking(const DockMessage &message) const
{
  const auto shipDbId     = message.getShipDbId();
  const auto shipEntityId = message.getShipEntityId();

  if (!m_shipService->tryUndock(shipDbId, shipEntityId))
  {
    warn("Failed to process undock message for ship " + str(shipDbId));
    return;
  }

  auto out = std::make_unique<DockMessage>(shipDbId, shipEntityId, false);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_messageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
