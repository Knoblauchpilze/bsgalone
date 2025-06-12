
#include "LoginMessageConsumer.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"
#include "PlayerListMessage.hh"

namespace bsgo {

LoginMessageConsumer::LoginMessageConsumer(LoginServicePtr loginService,
                                           ClientManagerShPtr clientManager,
                                           SystemProcessorMap systemProcessors,
                                           IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("login", {MessageType::LOGIN})
  , m_loginService(std::move(loginService))
  , m_systemProcessors(std::move(systemProcessors))
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

  m_messageQueue->pushMessage(std::move(out));
  publishLoadingMessages(clientId, *playerDbId);
}

void LoginMessageConsumer::publishLoadingMessages(const bsgo::Uuid clientId,
                                                  const bsgo::Uuid playerDbId) const
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

  auto started = std::make_unique<LoadingStartedMessage>();
  started->setClientId(clientId);
  maybeProcessor->second->pushMessage(std::move(started));

  auto players = std::make_unique<PlayerListMessage>(*maybeSystemDbId,
                                                     std::vector<PlayerListMessage::PlayerData>{});
  players->setClientId(clientId);
  maybeProcessor->second->pushMessage(std::move(players));

  auto finished = std::make_unique<LoadingFinishedMessage>();
  finished->setClientId(clientId);
  maybeProcessor->second->pushMessage(std::move(finished));
}

} // namespace bsgo
