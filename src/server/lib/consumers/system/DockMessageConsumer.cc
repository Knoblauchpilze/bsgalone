
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
  const auto shipDbId   = message.getShipDbId();
  const auto systemDbId = message.getSystemDbId();

  if (!m_shipService->tryDock(shipDbId))
  {
    warn("Failed to process dock message for ship " + str(shipDbId));
    return;
  }

  auto out = std::make_unique<DockMessage>(shipDbId, true, systemDbId);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_outputMessageQueue->pushMessage(std::move(out));

  auto started = std::make_unique<LoadingStartedMessage>(LoadingTransition::DOCK);
  started->setSystemDbId(systemDbId);
  started->copyClientIdIfDefined(message);
  m_systemMessageQueue->pushMessage(std::move(started));

  auto finished = std::make_unique<LoadingFinishedMessage>(LoadingTransition::DOCK);
  finished->setSystemDbId(systemDbId);
  finished->copyClientIdIfDefined(message);
  m_systemMessageQueue->pushMessage(std::move(finished));
}

void DockMessageConsumer::handleUndocking(const DockMessage &message) const
{
  const auto shipDbId   = message.getShipDbId();
  const auto systemDbId = message.getSystemDbId();

  auto out = std::make_unique<DockMessage>(shipDbId, false, systemDbId);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_outputMessageQueue->pushMessage(std::move(out));

  auto added = std::make_unique<EntityAddedMessage>(systemDbId);
  PlayerShipData data{.dbId = shipDbId};
  added->setShipData(data);
  m_systemMessageQueue->pushMessage(std::move(added));

  auto started = std::make_unique<LoadingStartedMessage>(LoadingTransition::UNDOCK);
  started->setSystemDbId(systemDbId);
  started->copyClientIdIfDefined(message);
  m_systemMessageQueue->pushMessage(std::move(started));

  auto finished = std::make_unique<LoadingFinishedMessage>(LoadingTransition::UNDOCK);
  finished->setSystemDbId(systemDbId);
  finished->copyClientIdIfDefined(message);
  m_systemMessageQueue->pushMessage(std::move(finished));
}

} // namespace bsgo
