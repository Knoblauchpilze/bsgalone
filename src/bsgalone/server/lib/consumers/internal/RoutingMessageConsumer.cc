
#include "RoutingMessageConsumer.hh"
#include "SystemProcessorUtils.hh"

namespace bsgalone::server {

RoutingMessageConsumer::RoutingMessageConsumer(SystemServiceShPtr systemService,
                                               core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("routing",
                            {core::MessageType::COMPONENT_SYNC, core::MessageType::TARGET})
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

void RoutingMessageConsumer::onEventReceived(const core::IMessage &message)
{
  switch (message.type())
  {
    case core::MessageType::COMPONENT_SYNC:
      handleComponentSyncMessage(message.as<core::ComponentSyncMessage>());
      break;
    case core::MessageType::TARGET:
      handleTargetMessage(message.as<core::TargetMessage>());
      break;
    default:
      error("Unsupported message " + str(message.type()) + " to route");
      break;
  }
}

void RoutingMessageConsumer::handleComponentSyncMessage(
  const core::ComponentSyncMessage &message) const
{
  const auto dbId       = message.getEntityDbId();
  const auto entityKind = message.getEntityKind();

  const auto maybeSystemDbId = tryFindSystemDbIdFromEntity(dbId, entityKind, *m_systemService);

  if (!maybeSystemDbId)
  {
    error("Failed to determine system for entity " + core::str(dbId) + " with kind "
          + str(entityKind));
  }

  auto out = message.clone();
  out->as<core::ComponentSyncMessage>().setSystemDbId(*maybeSystemDbId);
  m_outputMessageQueue->pushEvent(std::move(out));
}

void RoutingMessageConsumer::handleTargetMessage(const core::TargetMessage &message) const
{
  const auto dbId       = message.getSourceDbId();
  const auto entityKind = message.getSourceKind();

  const auto maybeSystemDbId = tryFindSystemDbIdFromEntity(dbId, entityKind, *m_systemService);

  if (!maybeSystemDbId)
  {
    error("Failed to determine system for entity " + core::str(dbId) + " with kind "
          + str(entityKind));
  }

  auto out = message.clone();
  out->as<core::TargetMessage>().setSystemDbId(*maybeSystemDbId);
  m_outputMessageQueue->pushEvent(std::move(out));
}

} // namespace bsgalone::server
