
#include "LootMessageConsumer.hh"
#include "LootMessage.hh"

namespace bsgo {

LootMessageConsumer::LootMessageConsumer(SystemServiceShPtr systemService,
                                         IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("loot", {MessageType::LOOT})
  , m_systemService(std::move(systemService))
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_systemService)
  {
    throw std::invalid_argument("Expected non null system service");
  }
  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void LootMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &loot = message.as<LootMessage>();

  const auto playerDbId   = loot.getPlayerDbId();
  const auto resourceDbId = loot.getResourceDbId();
  const auto amount       = loot.amount();

  if (!m_systemService->tryDistributeResource(playerDbId, resourceDbId, amount))
  {
    warn("Failed to process loot message for player " + str(playerDbId));
    return;
  }

  auto out = std::make_unique<LootMessage>(playerDbId, resourceDbId, amount);
  out->copyClientIdIfDefined(loot);
  m_messageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
