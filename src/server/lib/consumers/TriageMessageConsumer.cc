
#include "TriageMessageConsumer.hh"
#include "AbstractPlayerMessage.hh"
#include "AbstractSystemMessage.hh"
#include "NetworkMessage.hh"

namespace bsgo {

TriageMessageConsumer::TriageMessageConsumer(SystemQueueMap systemQueues,
                                             bsgalone::core::IMessageQueuePtr systemMessageQueue)
  : AbstractMessageConsumer("triage", bsgalone::core::allMessageTypesAsSet())
  , m_systemQueues(std::move(systemQueues))
  , m_systemQueue(std::move(systemMessageQueue))
{
  if (nullptr == m_systemQueue)
  {
    throw std::invalid_argument("Expected non null system message queue");
  }
}

void TriageMessageConsumer::onMessageReceived(const bsgalone::core::IMessage &message)
{
  if (discardMessageWithNoClient(message))
  {
    warn("Discarding message " + str(message.type()) + " with no client id");
    return;
  }

  if (tryRoutePlayerOrSystemMessage(message))
  {
    return;
  }

  switch (message.type())
  {
    case bsgalone::core::MessageType::SIGNUP:
    case bsgalone::core::MessageType::LOGIN:
    case bsgalone::core::MessageType::LOGOUT:
    case bsgalone::core::MessageType::JOIN_SHIP:
      handleSystemMessage(message);
      break;
    default:
      error("Unsupported message type to triage " + str(message.type()));
      break;
  }
}

bool TriageMessageConsumer::discardMessageWithNoClient(const bsgalone::core::IMessage &message) const
{
  if (!message.isA<bsgalone::core::NetworkMessage>())
  {
    return false;
  }

  return !message.as<bsgalone::core::NetworkMessage>().tryGetClientId().has_value();
}

bool TriageMessageConsumer::tryRoutePlayerOrSystemMessage(
  const bsgalone::core::IMessage &message) const
{
  std::optional<Uuid> maybeSystemDbId{};
  if (message.isA<bsgalone::core::AbstractPlayerMessage>())
  {
    maybeSystemDbId = message.as<bsgalone::core::AbstractPlayerMessage>().getSystemDbId();
  }
  if (message.isA<bsgalone::core::AbstractSystemMessage>())
  {
    maybeSystemDbId = message.as<bsgalone::core::AbstractSystemMessage>().getSystemDbId();
  }

  if (maybeSystemDbId)
  {
    routeSystemMessage(*maybeSystemDbId, message);
    return true;
  }

  return false;
}

void TriageMessageConsumer::handleSystemMessage(const bsgalone::core::IMessage &message) const
{
  m_systemQueue->pushMessage(message.clone());
}

void TriageMessageConsumer::routeSystemMessage(const Uuid systemDbId,
                                               const bsgalone::core::IMessage &message) const
{
  const auto maybeQueue = m_systemQueues.find(systemDbId);
  if (maybeQueue == m_systemQueues.cend())
  {
    error("Failed to process message " + str(message.type()),
          "Unsupported system " + str(systemDbId));
  }

  maybeQueue->second->pushMessage(message.clone());
}

} // namespace bsgo
