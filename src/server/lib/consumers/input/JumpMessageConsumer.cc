
#include "JumpMessageConsumer.hh"
#include "JumpMessage.hh"

namespace bsgo {

JumpMessageConsumer::JumpMessageConsumer(JumpServicePtr jumpService,
                                         SystemProcessorMap systemProcessors,
                                         IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("jump", {MessageType::JUMP})
  , m_jumpService(std::move(jumpService))
  , m_systemProcessors(std::move(systemProcessors))
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

  if (jump.validated())
  {
    return;
  }

  const auto shipDbId = jump.getShipDbId();

  if (!m_jumpService->tryJump(shipDbId))
  {
    warn("Failed to process jump message for ship " + str(shipDbId));
    return;
  }

  auto out = std::make_unique<JumpMessage>(shipDbId);
  out->validate();
  out->copyClientIdIfDefined(jump);
  m_messageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
