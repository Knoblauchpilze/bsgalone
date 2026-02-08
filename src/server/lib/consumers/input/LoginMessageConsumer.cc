
#include "LoginMessageConsumer.hh"

namespace bsgo {

LoginMessageConsumer::LoginMessageConsumer(LoginServicePtr loginService,
                                           bsgalone::server::ClientManagerShPtr clientManager,
                                           SystemQueueMap systemQueues,
                                           IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("login", {MessageType::LOGIN})
  , m_loginService(std::move(loginService))
  , m_clientManager(clientManager)
  , m_outputMessageQueue(outputMessageQueue)
  , m_helper(clientManager, std::move(systemQueues), outputMessageQueue)
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
  handleLogin(login);
}

void LoginMessageConsumer::handleLogin(const LoginMessage &message) const
{
  const auto clientId = message.getClientId();

  LoginService::LoginData data{
    .name     = message.getUserName(),
    .password = message.getPassword(),
    .role     = message.getGameRole(),
  };

  const auto maybePlayerDbId = m_loginService->tryLogin(data);

  const auto successfulLogin = maybePlayerDbId.has_value();

  std::optional<Uuid> maybeSystemDbId{};

  if (!successfulLogin)
  {
    warn("Failed to process login message for player " + data.name);
  }
  else
  {
    maybeSystemDbId = m_loginService->getPlayerSystemDbId(*maybePlayerDbId);
  }

  auto out = std::make_unique<LoginMessage>(data.role);
  out->setUserName(data.name);
  out->setPassword(data.password);
  if (maybePlayerDbId)
  {
    out->setPlayerDbId(*maybePlayerDbId);
  }
  if (maybeSystemDbId)
  {
    out->setSystemDbId(*maybeSystemDbId);
  }
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushMessage(std::move(out));

  if (successfulLogin)
  {
    m_helper.publishLoadingMessages(clientId, maybePlayerDbId.value(), maybeSystemDbId.value());
  }
}

} // namespace bsgo
