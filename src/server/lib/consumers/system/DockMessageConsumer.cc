
#include "DockMessageConsumer.hh"

namespace bsgo {

DockMessageConsumer::DockMessageConsumer(const Services &services, IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("dock", {MessageType::DOCK})
  , m_shipService(services.ship)
  , m_entityService(services.entity)
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_shipService)
  {
    throw std::invalid_argument("Expected non null ship service");
  }
  if (nullptr == m_entityService)
  {
    throw std::invalid_argument("Expected non null entity service");
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
  const auto shipDbId = message.getShipDbId();
  if (!m_shipService->tryDock(shipDbId))
  {
    warn("Failed to process dock message for ship " + str(shipDbId));
    return;
  }

  auto out = std::make_unique<DockMessage>(shipDbId, true);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_messageQueue->pushMessage(std::move(out));
}

void DockMessageConsumer::handleUndocking(const DockMessage &message) const
{
  const auto shipDbId = message.getShipDbId();
  if (!m_entityService->tryCreateShipEntity(shipDbId))
  {
    warn("Failed to process undock message for ship " + str(shipDbId));
    return;
  }

  auto out = std::make_unique<DockMessage>(shipDbId, false);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_messageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
