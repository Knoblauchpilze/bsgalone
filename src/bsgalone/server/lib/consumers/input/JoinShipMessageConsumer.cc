
#include "JoinShipMessageConsumer.hh"

namespace bsgalone::server {

JoinShipMessageConsumer::JoinShipMessageConsumer(PlayerServicePtr playerService,
                                                 core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("join", {core::MessageType::JOIN_SHIP})
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

void JoinShipMessageConsumer::onEventReceived(const core::IMessage &message)
{
  const auto &joinShip = message.as<core::JoinShipMessage>();
  handleJoinShip(joinShip);
}

void JoinShipMessageConsumer::handleJoinShip(const core::JoinShipMessage &message) const
{
  if (!m_playerService->tryJoinShip(message.getPlayerDbId(), message.getShipDbId()))
  {
    warn("Failed to process join ship message",
         "Player " + core::str(message.getPlayerDbId()) + " cannot join desired ship "
           + core::str(message.getShipDbId()));
    return;
  }

  m_outputMessageQueue->pushEvent(message.clone());
}

} // namespace bsgalone::server
