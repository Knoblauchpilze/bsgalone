
#include "JoinShipMessageConsumer.hh"

namespace bsgo {

JoinShipMessageConsumer::JoinShipMessageConsumer(
  PlayerServicePtr playerService,
  bsgalone::core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("join", {MessageType::JOIN_SHIP})
  , m_playerService(std::move(playerService))
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_playerService)
  {
    throw std::invalid_argument("Expected non null player service");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void JoinShipMessageConsumer::onMessageReceived(const bsgalone::core::IMessage &message)
{
  const auto &joinShip = message.as<JoinShipMessage>();
  handleJoinShip(joinShip);
}

void JoinShipMessageConsumer::handleJoinShip(const JoinShipMessage &message) const
{
  if (!m_playerService->tryJoinShip(message.getPlayerDbId(), message.getShipDbId()))
  {
    warn("Failed to process join ship message",
         "Player " + str(message.getPlayerDbId()) + " cannot join desired ship "
           + str(message.getShipDbId()));
    return;
  }

  m_outputMessageQueue->pushMessage(message.clone());
}

} // namespace bsgo
