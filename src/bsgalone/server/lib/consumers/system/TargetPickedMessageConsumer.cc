
#include "TargetPickedMessageConsumer.hh"
#include "TargetMessage.hh"

namespace bsgalone::server {

TargetPickedMessageConsumer::TargetPickedMessageConsumer(
  const Services &services,
  core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("target", {core::MessageType::TARGET_PICKED})
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

void TargetPickedMessageConsumer::onEventReceived(const core::IMessage &message)
{
  const auto &targetMessage = message.as<core::TargetPickedMessage>();

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
      warn("Failed to process target message for " + core::str(data.sourceDbId) + " "
           + core::str(data.sourceKind));
    }
    return;
  }

  core::Target target{
    .sourceDbId = data.sourceDbId,
    .sourceKind = data.sourceKind,
    .targetDbId = res.targetDbId,
    .targetKind = res.targetKind,
  };

  auto out = std::make_unique<core::TargetMessage>(target, position);
  broadcastMessageToSystem(std::move(out));
}

namespace {
auto getSystemDbIdForSource(const core::Uuid dbId,
                            const core::EntityKind kind,
                            const ShipService &shipService) -> core::Uuid
{
  switch (kind)
  {
    case core::EntityKind::SHIP:
      return shipService.getSystemDbIdForShip(dbId);
    default:
      throw std::invalid_argument("Unsupported entity kind " + str(kind)
                                  + "to get system identifier");
  }
}
} // namespace

void TargetPickedMessageConsumer::broadcastMessageToSystem(
  std::unique_ptr<core::TargetMessage> message)
{
  const auto systemDbId = getSystemDbIdForSource(message->getSourceDbId(),
                                                 message->getSourceKind(),
                                                 *m_shipService);
  message->setSystemDbId(systemDbId);

  m_outputMessageQueue->pushEvent(std::move(message));
}

} // namespace bsgalone::server
