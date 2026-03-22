
#include "LoginRequestConsumer.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"
#include "LoginMessage.hh"
#include "LoginRequest.hh"

namespace bsgalone::server {

LoginRequestConsumer::LoginRequestConsumer(LoginServicePtr loginService,
                                           SystemQueueMap systemQueues,
                                           core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("login", {core::MessageType::LOGIN_REQUEST})
  , m_loginService(std::move(loginService))
  , m_systemQueues(std::move(systemQueues))
  , m_outputMessageQueue(outputMessageQueue)
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

void LoginRequestConsumer::onEventReceived(const core::IMessage &message)
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

    publishLoadingMessages(*maybeClientId, *maybePlayerDbId, systemDbId);
  }
  else
  {
    warn("Failed to process login message for player " + data.name);

    auto out = std::make_unique<core::LoginMessage>(*maybeClientId);
    m_outputMessageQueue->pushEvent(std::move(out));
  }
}

void LoginRequestConsumer::publishLoadingMessages(const core::Uuid clientId,
                                                  const core::Uuid playerDbId,
                                                  const core::Uuid systemDbId) const
{
  const auto maybeQueue = m_systemQueues.find(systemDbId);
  if (maybeQueue == m_systemQueues.cend())
  {
    error("Failed to process login message for " + core::str(playerDbId),
          "Unknown system " + core::str(systemDbId));
  }

  auto started = std::make_unique<core::LoadingStartedMessage>(core::LoadingTransition::LOGIN,
                                                               playerDbId);
  started->setSystemDbId(systemDbId);
  started->setClientId(clientId);
  maybeQueue->second->pushEvent(std::move(started));

  auto finished = std::make_unique<core::LoadingFinishedMessage>(core::LoadingTransition::LOGIN,
                                                                 playerDbId);
  finished->setSystemDbId(systemDbId);
  finished->setClientId(clientId);
  maybeQueue->second->pushEvent(std::move(finished));
}

} // namespace bsgalone::server
