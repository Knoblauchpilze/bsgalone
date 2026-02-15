
#include "TargetMessageConsumer.hh"
#include "TargetMessage.hh"

namespace bsgo {

TargetMessageConsumer::TargetMessageConsumer(const Services &services,
                                             bsgalone::core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("target", {MessageType::TARGET})
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

void TargetMessageConsumer::onMessageReceived(const bsgalone::core::IMessage &message)
{
  const auto &target = message.as<TargetMessage>();

  const auto position       = target.getPosition();
  const auto targetDbIdHint = target.tryGetTargetDbId();

  const ShipService::TargetAcquiringData acquiringData{
    .sourceDbId     = target.getSourceDbId(),
    .sourceKind     = target.getSourceKind(),
    .position       = position,
    .targetDbIdHint = targetDbIdHint,
    .targetKindHint = target.tryGetTargetKind(),
  };
  const auto res = m_shipService->tryAcquireTarget(acquiringData);
  if (!res.success)
  {
    // Only print a warning in case there was a hint in the message. In case a hint
    // is provided and we can't find it, it means that there's a discrepency between
    // what the client app knows and what the server knows.
    if (targetDbIdHint)
    {
      warn("Failed to process target message for " + str(target.getSourceKind()) + " "
           + str(target.getSourceDbId()));
    }
    return;
  }

  TargetData data{
    .sourceDbId = target.getSourceDbId(),
    .sourceKind = target.getSourceKind(),
    .targetDbId = res.targetDbId,
    .targetKind = res.targetKind,
  };

  auto out = std::make_unique<TargetMessage>(data, position);
  broadcastMessageToSystem(std::move(out));
}

namespace {
auto getSystemDbIdForSource(const Uuid dbId, const EntityKind kind, const ShipService &shipService)
  -> Uuid
{
  switch (kind)
  {
    case EntityKind::SHIP:
      return shipService.getSystemDbIdForShip(dbId);
    default:
      throw std::invalid_argument("Unsupported entity kind " + str(kind)
                                  + "to get system identifier");
  }
}
} // namespace

void TargetMessageConsumer::broadcastMessageToSystem(std::unique_ptr<TargetMessage> message)
{
  const auto systemDbId = getSystemDbIdForSource(message->getSourceDbId(),
                                                 message->getSourceKind(),
                                                 *m_shipService);
  message->setSystemDbId(systemDbId);

  m_outputMessageQueue->pushMessage(std::move(message));
}

} // namespace bsgo
