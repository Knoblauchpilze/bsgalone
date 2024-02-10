
#include "JumpMessageConsumer.hh"

namespace bsgo {

JumpMessageConsumer::JumpMessageConsumer(const Services &services, IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("jump", {MessageType::JUMP})
  , m_jumpService(services.jump)
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_jumpService)
  {
    throw std::invalid_argument("Expected non null jump service");
  }
  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void JumpMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &jump = message.as<JumpMessage>();

  if (!jump.validated())
  {
    handleJump(jump);
  }
}

void JumpMessageConsumer::handleJump(const JumpMessage &message) const
{
  const auto shipDbId = message.getShipDbId();

  if (!m_jumpService->tryJump(shipDbId))
  {
    warn("Failed to process jump message for ship " + str(shipDbId));
    return;
  }

  auto out = std::make_unique<JumpMessage>(shipDbId);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_messageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
