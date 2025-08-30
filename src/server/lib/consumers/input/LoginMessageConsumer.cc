
#include "LoginMessageConsumer.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"

namespace bsgo {

LoginMessageConsumer::LoginMessageConsumer(LoginServicePtr loginService,
                                           ClientManagerShPtr clientManager,
                                           SystemProcessorMap systemProcessors,
                                           IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("login", {MessageType::LOGIN})
  , m_loginService(std::move(loginService))
  , m_systemProcessors(std::move(systemProcessors))
  , m_clientManager(std::move(clientManager))
  , m_outputMessageQueue(outputMessageQueue)
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

  m_outputMessageQueue->pushMessage(std::move(out));
  publishLoadingMessages(clientId, *playerDbId);
}

void LoginMessageConsumer::publishLoadingMessages(const Uuid clientId, const Uuid playerDbId) const
{
  const auto maybeSystemDbId = m_clientManager->tryGetSystemForClient(clientId);
  if (!maybeSystemDbId)
  {
    error("Failed to process login message for " + str(playerDbId), "No associated system");
  }

  const auto maybeProcessor = m_systemProcessors.find(*maybeSystemDbId);
  if (maybeProcessor == m_systemProcessors.cend())
  {
    error("Failed to process login message for " + str(playerDbId),
          "Unknown system " + str(*maybeSystemDbId));
  }

  auto started = std::make_unique<LoadingStartedMessage>(LoadingTransition::LOGIN);
  started->setSystemDbId(*maybeSystemDbId);
  started->setPlayerDbId(playerDbId);
  started->setClientId(clientId);
  maybeProcessor->second->pushMessage(std::move(started));

  auto finished = std::make_unique<LoadingFinishedMessage>(LoadingTransition::LOGIN);
  finished->setSystemDbId(*maybeSystemDbId);
  finished->setPlayerDbId(playerDbId);
  finished->setClientId(clientId);
  maybeProcessor->second->pushMessage(std::move(finished));
}
} // namespace bsgo
