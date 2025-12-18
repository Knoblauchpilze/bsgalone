
#include "JoinShipMessageConsumer.hh"

namespace bsgo {

JoinShipMessageConsumer::JoinShipMessageConsumer(IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("join", {MessageType::JOIN_SHIP})
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void JoinShipMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &joinShip = message.as<JoinShipMessage>();

  if (!joinShip.validated())
  {
    handleJoinShip(joinShip);
  }
}

void JoinShipMessageConsumer::handleJoinShip(const JoinShipMessage & /*message*/) const
{
  warn("should handle join ship");
}

} // namespace bsgo
