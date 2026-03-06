
#include "LootMessageConsumer.hh"
#include "LootMessage.hh"

namespace bsgalone::server {

LootMessageConsumer::LootMessageConsumer(SystemServiceShPtr systemService,
                                         core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("loot", {core::MessageType::LOOT})
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

void LootMessageConsumer::onEventReceived(const core::IMessage &message)
{
  const auto &loot = message.as<core::LootMessage>();

  const auto playerDbId   = loot.getPlayerDbId();
  const auto resourceDbId = loot.getResourceDbId();
  const auto amount       = loot.amount();

  if (!m_systemService->tryDistributeResource(playerDbId, resourceDbId, amount))
  {
    warn("Failed to process loot message for player " + core::str(playerDbId));
    return;
  }

  auto out = std::make_unique<core::LootMessage>(playerDbId, resourceDbId, amount);
  out->copyClientIdIfDefined(loot);
  m_outputMessageQueue->pushEvent(std::move(out));
}

} // namespace bsgalone::server
