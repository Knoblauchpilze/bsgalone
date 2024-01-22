
#include "JumpMessageConsumer.hh"
#include "JumpMessage.hh"

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
    handleJump(jump.getShipDbId(), jump.getShipEntityId());
  }
}

void JumpMessageConsumer::handleJump(const Uuid &shipDbId, const Uuid &shipEntityId) const
{
  if (!m_jumpService->tryJump(shipDbId, shipEntityId))
  {
    warn("Failed to process jump message for ship " + str(shipDbId));
    return;
  }

  auto message = std::make_unique<JumpMessage>(shipDbId, shipEntityId);
  message->validate();
  m_messageQueue->pushMessage(std::move(message));
}

} // namespace bsgo
