
#include "LogoutMessageConsumer.hh"

namespace bsgo {

LogoutMessageConsumer::LogoutMessageConsumer(ClientManagerShPtr clientManager,
                                             IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("logout", {MessageType::LOGOUT})
  , m_clientManager(std::move(clientManager))
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void LogoutMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &logout = message.as<LogoutMessage>();

  if (!logout.validated())
  {
    handleLogout(logout);
  }
}

void LogoutMessageConsumer::handleLogout(const LogoutMessage &message) const
{
  const auto playerDbId = message.getPlayerDbId();

  warn("should handle logout");
  // m_clientManager->registerPlayer(message.getClientId(), *playerDbId, systsemDbId);

  auto out = std::make_unique<LogoutMessage>(playerDbId);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_messageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
