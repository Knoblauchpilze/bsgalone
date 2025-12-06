
#include "RoutingMessageConsumer.hh"
#include "SystemProcessorUtils.hh"

namespace bsgo {

RoutingMessageConsumer::RoutingMessageConsumer(SystemServiceShPtr systemService,
                                               IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("routing", {MessageType::COMPONENT_SYNC, MessageType::TARGET})
  , m_systemService(std::move(systemService))
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
  if (nullptr == m_systemService)
  {
    throw std::invalid_argument("Expected non null system service");
  }
}

void RoutingMessageConsumer::onMessageReceived(const IMessage &message)
{
  switch (message.type())
  {
    case MessageType::COMPONENT_SYNC:
      handleComponentSyncMessage(message.as<ComponentSyncMessage>());
      break;
    case MessageType::TARGET:
      handleTargetMessage(message.as<TargetMessage>());
      break;
    default:
      error("Unsupported message " + str(message.type()) + " to route");
      break;
  }
}

void RoutingMessageConsumer::handleComponentSyncMessage(const ComponentSyncMessage &message) const
{
  const auto dbId       = message.getEntityDbId();
  const auto entityKind = message.getEntityKind();

  const auto maybeSystemDbId = tryFindSystemDbIdFromEntity(dbId, entityKind, *m_systemService);

  if (!maybeSystemDbId)
  {
    error("Failed to determine system for entity " + str(dbId) + " with kind " + str(entityKind));
  }

  auto out = message.clone();
  out->as<ComponentSyncMessage>().setSystemDbId(*maybeSystemDbId);
  m_outputMessageQueue->pushMessage(std::move(out));
}

void RoutingMessageConsumer::handleTargetMessage(const TargetMessage &message) const
{
  const auto dbId       = message.getSourceDbId();
  const auto entityKind = message.getSourceKind();

  const auto maybeSystemDbId = tryFindSystemDbIdFromEntity(dbId, entityKind, *m_systemService);

  if (!maybeSystemDbId)
  {
    error("Failed to determine system for entity " + str(dbId) + " with kind " + str(entityKind));
  }

  auto out = message.clone();
  out->as<TargetMessage>().setSystemDbId(*maybeSystemDbId);
  m_outputMessageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
