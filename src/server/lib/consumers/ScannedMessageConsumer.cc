
#include "ScannedMessageConsumer.hh"

namespace bsgo {

ScannedMessageConsumer::ScannedMessageConsumer(IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("scanned", {MessageType::SCANNED})
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void ScannedMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &scan = message.as<ScannedMessage>();

  if (!scan.validated())
  {
    handleScanOperation(scan);
  }
}

void ScannedMessageConsumer::handleScanOperation(const ScannedMessage &message) const
{
  const auto asteroidDbId = message.getAsteroidDbId();
  /// TODO: Perform some sort of validation.

  auto out = std::make_unique<ScannedMessage>(asteroidDbId);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_messageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
