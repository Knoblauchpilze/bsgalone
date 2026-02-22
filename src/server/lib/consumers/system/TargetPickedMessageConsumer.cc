
#include "TargetPickedMessageConsumer.hh"
#include "TargetMessage.hh"

namespace bsgo {

TargetPickedMessageConsumer::TargetPickedMessageConsumer(
  const Services &services,
  bsgalone::core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("target", {bsgalone::core::MessageType::TARGET_PICKED})
  , m_shipService(services.ship)
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_shipService)
  {
    throw std::invalid_argument("Expected non null ship service");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void TargetPickedMessageConsumer::onMessageReceived(const bsgalone::core::IMessage &message)
{
  const auto &targetMessage = message.as<bsgalone::core::TargetPickedMessage>();

  const auto data     = targetMessage.getTargetData();
  const auto position = targetMessage.getPosition();

  const ShipService::TargetAcquiringData acquiringData{
    .sourceDbId     = data.sourceDbId,
    .sourceKind     = data.sourceKind,
    .position       = position,
    .targetDbIdHint = data.targetDbId,
    .targetKindHint = data.targetKind,
  };
  const auto res = m_shipService->tryAcquireTarget(acquiringData);
  if (!res.success)
  {
    // Only print a warning in case there was a hint in the message. In case a hint
    // is provided and we can't find it, it means that there's a discrepency between
    // what the client app knows and what the server knows.
    if (data.targetDbId)
    {
      warn("Failed to process target message for " + str(data.sourceDbId) + " "
           + str(data.sourceKind));
    }
    return;
  }

  bsgalone::core::Target target{
    .sourceDbId = data.sourceDbId,
    .sourceKind = data.sourceKind,
    .targetDbId = res.targetDbId,
    .targetKind = res.targetKind,
  };

  auto out = std::make_unique<TargetMessage>(target, position);
  broadcastMessageToSystem(std::move(out));
}

namespace {
auto getSystemDbIdForSource(const Uuid dbId,
                            const bsgalone::core::EntityKind kind,
                            const ShipService &shipService) -> Uuid
{
  switch (kind)
  {
    case bsgalone::core::EntityKind::SHIP:
      return shipService.getSystemDbIdForShip(dbId);
    default:
      throw std::invalid_argument("Unsupported entity kind " + str(kind)
                                  + "to get system identifier");
  }
}
} // namespace

void TargetPickedMessageConsumer::broadcastMessageToSystem(std::unique_ptr<TargetMessage> message)
{
  const auto systemDbId = getSystemDbIdForSource(message->getSourceDbId(),
                                                 message->getSourceKind(),
                                                 *m_shipService);
  message->setSystemDbId(systemDbId);

  m_outputMessageQueue->pushMessage(std::move(message));
}

} // namespace bsgo
