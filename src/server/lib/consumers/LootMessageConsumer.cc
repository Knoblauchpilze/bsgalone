
#include "LootMessageConsumer.hh"

namespace bsgo {

LootMessageConsumer::LootMessageConsumer(CombatServicePtr combatService,
                                         IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("loot", {MessageType::LOOT})
  , m_combatService(std::move(combatService))
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_combatService)
  {
    throw std::invalid_argument("Expected non null combat service");
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

  if (!m_combatService->tryDistributeResource(playerDbId, resourceDbId, amount))
  {
    warn("Failed to process loot message for player " + str(playerDbId));
    return;
  }

  auto out = std::make_unique<LootMessage>(playerDbId, resourceDbId, amount);
  out->copyClientIdIfDefined(loot);
  m_messageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
