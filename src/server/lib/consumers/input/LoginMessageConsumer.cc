
#include "LoginMessageConsumer.hh"

namespace bsgo {

LoginMessageConsumer::LoginMessageConsumer(LoginServicePtr loginService,
                                           ClientManagerShPtr clientManager,
                                           SystemProcessorMap systemProcessors,
                                           IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("login", {MessageType::LOGIN})
  , m_loginService(std::move(loginService))
  , m_clientManager(clientManager)
  , m_outputMessageQueue(outputMessageQueue)
  , m_helper(clientManager, std::move(systemProcessors), outputMessageQueue)
{
  if (nullptr == m_loginService)
  {
    throw std::invalid_argument("Expected non null login service");
  }
  if (nullptr == m_clientManager)
  {
    throw std::invalid_argument("Expected non null client manager");
  }
  if (nullptr == m_outputMessageQueue)
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
  const auto clientId = message.getClientId();
  const auto name     = message.getUserName();
  const auto password = message.getUserPassword();

  const auto maybePlayerDbId = m_loginService->tryLogin(name, password);

  const auto successfulLogin = maybePlayerDbId.has_value();

  if (!successfulLogin)
  {
    warn("Failed to process login message for player " + name);
  }
  else
  {
    const auto systsemDbId = m_loginService->getPlayerSystemDbId(*maybePlayerDbId);
    m_clientManager->registerPlayer(message.getClientId(), *maybePlayerDbId, systsemDbId);
  }

  auto out = std::make_unique<LoginMessage>(name, password);
  if (maybePlayerDbId)
  {
    out->setPlayerDbId(*maybePlayerDbId);
  }
  out->validate();
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushMessage(std::move(out));

  if (successfulLogin)
  {
    m_helper.publishLoadingMessages(clientId, *maybePlayerDbId);
  }
}

} // namespace bsgo
