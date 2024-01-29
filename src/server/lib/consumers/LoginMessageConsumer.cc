
#include "LoginMessageConsumer.hh"

namespace bsgo {

LoginMessageConsumer::LoginMessageConsumer(const Services &services,
                                           IMessageQueue *const messageQueue,
                                           const PlayerLoginCallback &playerLoginCallback)
  : AbstractMessageConsumer("login", {MessageType::LOGIN})
  , m_loginService(services.login)
  , m_messageQueue(messageQueue)
  , m_playerLoginCallback(playerLoginCallback)
{
  if (nullptr == m_loginService)
  {
    throw std::invalid_argument("Expected non null login service");
  }
  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void LoginMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &login = message.as<LoginMessage>();

  if (!login.validated())
  {
    handleLogin(login);
  }
}

void LoginMessageConsumer::handleLogin(const LoginMessage &message) const
{
  const auto name     = message.getUserName();
  const auto password = message.getUserPassword();

  const auto playerDbId = m_loginService->tryLogin(name, password);

  if (!playerDbId)
  {
    warn("Failed to process login message for player " + name);
  }

  m_playerLoginCallback(message.getClientId(), *playerDbId);

  auto out = std::make_unique<LoginMessage>(name, password, playerDbId);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_messageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
