
#include "TriageMessageConsumer.hh"
#include "NetworkMessage.hh"

namespace bsgo {

TriageMessageConsumer::TriageMessageConsumer(std::vector<SystemProcessorShPtr> systemProcessors,
                                             ClientManagerShPtr clientManager,
                                             IMessageQueuePtr systemMessageQueue)
  : AbstractMessageConsumer("triage", allMessageTypesAsSet())
  , m_clientManager(std::move(clientManager))
  , m_systemProcessors(std::move(systemProcessors))
  , m_systemQueue(std::move(systemMessageQueue))
{
  if (nullptr == m_systemQueue)
  {
    throw std::invalid_argument("Expected non null system message queue");
  }
}

void TriageMessageConsumer::onMessageReceived(const IMessage &message)
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
      handleSystemMessage(message);
      break;
    case MessageType::DOCK:
    case MessageType::EQUIP:
    case MessageType::HANGAR:
    case MessageType::JUMP:
    case MessageType::JUMP_CANCELLED:
    case MessageType::JUMP_REQUESTED:
    case MessageType::PURCHASE:
    case MessageType::SLOT:
      triagePlayerMessage(message);
      break;
    default:
      error("Unsupported message type to triage " + str(message.type()));
      break;
  }
}

bool TriageMessageConsumer::discardMessageWithNoClient(const IMessage &message) const
{
  if (!message.isA<NetworkMessage>())
  {
    return false;
  }

  return !message.as<NetworkMessage>().tryGetClientId().has_value();
}

void TriageMessageConsumer::handleSystemMessage(const IMessage &message) const
{
  m_systemQueue->pushMessage(message.clone());
}

void TriageMessageConsumer::triagePlayerMessage(const IMessage &message) const
{
  /// TODO: We need a clone to send the messages to the processor
  warn("should triage player message " + str(message.type()));
}

} // namespace bsgo
