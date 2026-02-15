
#include "TriageMessageConsumer.hh"
#include "NetworkMessage.hh"

namespace bsgo {

TriageMessageConsumer::TriageMessageConsumer(bsgalone::server::ClientManagerShPtr clientManager,
                                             SystemQueueMap systemQueues,
                                             bsgalone::core::IMessageQueuePtr systemMessageQueue)
  : AbstractMessageConsumer("triage", allMessageTypesAsSet())
  , m_clientManager(std::move(clientManager))
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

  switch (message.type())
  {
    case MessageType::SIGNUP:
    case MessageType::LOGIN:
    case MessageType::LOGOUT:
    case MessageType::JOIN_SHIP:
      handleSystemMessage(message);
      break;
    case MessageType::DOCK:
    case MessageType::EQUIP:
    case MessageType::HANGAR:
    case MessageType::JUMP_CANCELLED:
    case MessageType::JUMP_REQUESTED:
    case MessageType::PURCHASE:
    case MessageType::SLOT:
    case MessageType::TARGET:
    case MessageType::VELOCITY:
      triagePlayerMessage(message);
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

void TriageMessageConsumer::handleSystemMessage(const bsgalone::core::IMessage &message) const
{
  m_systemQueue->pushMessage(message.clone());
}

void TriageMessageConsumer::triagePlayerMessage(const bsgalone::core::IMessage &message) const
{
  if (!message.isA<bsgalone::core::NetworkMessage>())
  {
    broadcastMessage(message);
    return;
  }

  const auto clientId    = message.as<bsgalone::core::NetworkMessage>().getClientId();
  const auto maybeSystem = m_clientManager->tryGetSystemForClient(clientId);

  if (!maybeSystem)
  {
    error("Failed to process message " + str(message.type()),
          "Client " + str(clientId) + " does not have a registered system");
  }

  const auto maybeQueue = m_systemQueues.find(*maybeSystem);
  if (maybeQueue == m_systemQueues.cend())
  {
    error("Failed to process message " + str(message.type()),
          "Unsupported system " + str(*maybeSystem));
  }

  maybeQueue->second->pushMessage(message.clone());
}

void TriageMessageConsumer::broadcastMessage(const bsgalone::core::IMessage &message) const
{
  debug("Brodcasting message " + str(message.type()));

  for (const auto &[_, queue] : m_systemQueues)
  {
    queue->pushMessage(message.clone());
  }
}

} // namespace bsgo
