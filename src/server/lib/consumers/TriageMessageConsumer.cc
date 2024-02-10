
#include "TriageMessageConsumer.hh"
#include "NetworkMessage.hh"

namespace bsgo {

TriageMessageConsumer::TriageMessageConsumer(
  const std::vector<SystemProcessorShPtr> &systemProcessors,
  ClientManagerShPtr clientManager,
  IMessageQueuePtr systemMessageQueue)
  : AbstractMessageConsumer("triage", allMessageTypesAsSet())
  , m_clientManager(std::move(clientManager))
  , m_systemQueue(std::move(systemMessageQueue))
{
  if (nullptr == m_systemQueue)
  {
    throw std::invalid_argument("Expected non null system message queue");
  }

  for (const auto &system : systemProcessors)
  {
    const auto res = m_systemProcessors.try_emplace(system->getSystemDbId(), system);
    if (!res.second)
    {
      throw std::invalid_argument("Failed to register duplicated system "
                                  + str(system->getSystemDbId()));
    }
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
    case MessageType::TARGET:
    case MessageType::VELOCITY:
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
  if (!message.isA<NetworkMessage>())
  {
    broadcastMessage(message);
    return;
  }

  const auto clientId    = message.as<NetworkMessage>().getClientId();
  const auto maybeSystem = m_clientManager->tryGetSystemForClient(clientId);

  if (!maybeSystem)
  {
    error("Failed to process message " + str(message.type()),
          "Client " + str(clientId) + " does not have a registered system");
  }

  const auto maybeProcessor = m_systemProcessors.find(*maybeSystem);
  if (maybeProcessor == m_systemProcessors.cend())
  {
    error("Failed to process message " + str(message.type()),
          "Unsupported system " + str(*maybeSystem));
  }

  maybeProcessor->second->pushMessage(message.clone());
}

void TriageMessageConsumer::broadcastMessage(const IMessage &message) const
{
  debug("Brodcasting message " + str(message.type()));

  for (const auto &[_, processor] : m_systemProcessors)
  {
    processor->pushMessage(message.clone());
  }
}

} // namespace bsgo
