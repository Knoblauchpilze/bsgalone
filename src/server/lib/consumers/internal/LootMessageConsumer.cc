
#include "LootMessageConsumer.hh"
#include "LootMessage.hh"

namespace bsgo {

LootMessageConsumer::LootMessageConsumer(SystemServiceShPtr systemService,
                                         bsgalone::core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("loot", {MessageType::LOOT})
  , m_systemService(std::move(systemService))
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_systemService)
  {
    throw std::invalid_argument("Expected non null system service");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void LootMessageConsumer::onMessageReceived(const bsgalone::core::IMessage &message)
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
  m_outputMessageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
