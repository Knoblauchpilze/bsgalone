
#include "LoginMessageConsumer.hh"

namespace bsgo {

LoginMessageConsumer::LoginMessageConsumer(LoginServicePtr loginService,
                                           ClientManagerShPtr clientManager,
                                           IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("login", {MessageType::LOGIN})
  , m_loginService(std::move(loginService))
  , m_clientManager(std::move(clientManager))
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_loginService)
  {
    throw std::invalid_argument("Expected non null login service");
  }
  if (nullptr == m_clientManager)
  {
    throw std::invalid_argument("Expected non null client manager");
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
  else
  {
    const auto systsemDbId = m_loginService->getPlayerSystemDbId(*playerDbId);
    m_clientManager->registerPlayer(message.getClientId(), *playerDbId, systsemDbId);
  }

  auto out = std::make_unique<LoginMessage>(name, password, playerDbId);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_messageQueue->pushMessage(std::move(out));

  // TODO: We could have a similar logic as in LogoutMessageConsumer. Using the client
  // manager we could get the system id of the player and send a PlayerListMessage there
  // We also have access to the client id so it can be used to send the players' list
  // only to the relevant client
}

} // namespace bsgo
