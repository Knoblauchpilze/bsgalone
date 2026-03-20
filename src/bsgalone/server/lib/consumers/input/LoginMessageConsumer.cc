
#include "LoginMessageConsumer.hh"
#include "LoginMessage.hh"
#include "LoginRequest.hh"

namespace bsgalone::server {

LoginMessageConsumer::LoginMessageConsumer(LoginServicePtr loginService,
                                           SystemQueueMap systemQueues,
                                           core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("login", {core::MessageType::LOGIN_REQUEST})
  , m_loginService(std::move(loginService))
  , m_outputMessageQueue(outputMessageQueue)
  , m_helper(std::move(systemQueues), outputMessageQueue)
{
  if (nullptr == m_loginService)
  {
    throw std::invalid_argument("Expected non null login service");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void LoginMessageConsumer::onEventReceived(const core::IMessage &message)
{
  const auto &request = message.as<core::LoginRequest>();

  const auto maybeClientId = request.tryGetClientId();
  if (!maybeClientId.has_value())
  {
    warn("Failed to process login request without client identifier");
    return;
  }

  LoginService::LoginData data{
    .name     = request.getUsername(),
    .password = request.getPassword(),
    .role     = request.getRole(),
  };

  const auto maybePlayerDbId = m_loginService->tryLogin(data);
  const auto successfulLogin = maybePlayerDbId.has_value();

  if (successfulLogin)
  {
    const auto systemDbId = m_loginService->getPlayerSystemDbId(*maybePlayerDbId);

    auto out = std::make_unique<core::LoginMessage>(*maybeClientId);
    out->setPlayerDbId(*maybePlayerDbId);
    out->setRole(data.role);
    out->setSystemDbId(systemDbId);
    m_outputMessageQueue->pushEvent(std::move(out));

    m_helper.publishLoadingMessages(*maybeClientId, *maybePlayerDbId, systemDbId);
  }
  else
  {
    warn("Failed to process login message for player " + data.name);

    auto out = std::make_unique<core::LoginMessage>(*maybeClientId);
    m_outputMessageQueue->pushEvent(std::move(out));
  }
}

} // namespace bsgalone::server
