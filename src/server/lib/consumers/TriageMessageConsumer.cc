
#include "TriageMessageConsumer.hh"
#include "AbstractPlayerMessage.hh"
#include "AbstractSystemMessage.hh"
#include "NetworkMessage.hh"

namespace bsgo {

const std::unordered_set<bsgalone::core::MessageType> RELEVANT_MESSAGES = {
  bsgalone::core::MessageType::DOCK,
  bsgalone::core::MessageType::EQUIP,
  bsgalone::core::MessageType::JUMP_CANCELLED,
  bsgalone::core::MessageType::JUMP_REQUESTED,
  bsgalone::core::MessageType::PURCHASE,
  bsgalone::core::MessageType::SHIP_SELECTED,
  bsgalone::core::MessageType::SLOT,
  bsgalone::core::MessageType::TARGET_PICKED,
  bsgalone::core::MessageType::VELOCITY,
};

TriageMessageConsumer::TriageMessageConsumer(SystemQueueMap systemQueues)
  : AbstractMessageConsumer("triage", RELEVANT_MESSAGES)
  , m_systemQueues(std::move(systemQueues))
{}

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

  error("Unsupported message type to triage " + str(message.type()));
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
