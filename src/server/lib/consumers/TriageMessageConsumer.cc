
#include "TriageMessageConsumer.hh"

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

void TriageMessageConsumer::handleSystemMessage(const IMessage &message) const
{
  warn("should triage system message " + str(message.type()));
}

void TriageMessageConsumer::triagePlayerMessage(const IMessage &message) const
{
  warn("should triage player message " + str(message.type()));
}

} // namespace bsgo
