
#include "SignupMessageConsumer.hh"

namespace bsgo {

SignupMessageConsumer::SignupMessageConsumer(SignupServicePtr signupService,
                                             ClientManagerShPtr clientManager,
                                             IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("signup", {MessageType::SIGNUP})
  , m_signupService(std::move(signupService))
  , m_clientManager(std::move(clientManager))
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_signupService)
  {
    throw std::invalid_argument("Expected non null signup service");
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

void SignupMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &signup = message.as<SignupMessage>();

  if (!signup.validated())
  {
    handleSignup(signup);
  }
}

void SignupMessageConsumer::handleSignup(const SignupMessage &message) const
{
  const auto name     = message.getUserName();
  const auto password = message.getUserPassword();
  const auto faction  = message.getFaction();

  const auto maybePlayer = m_signupService->trySignup(name, password, faction);
  if (!maybePlayer)
  {
    warn("Failed to process signup message for player " + name);
  }
  else
  {
    const auto playerDbId  = maybePlayer->id;
    const auto systsemDbId = m_signupService->getPlayerSystemDbId(playerDbId);
    m_clientManager->registerPlayer(message.getClientId(), playerDbId, systsemDbId);
  }

  auto out = std::make_unique<SignupMessage>(name,
                                             password,
                                             faction,
                                             maybePlayer.has_value() ? maybePlayer->id
                                                                     : std::optional<Uuid>{});
  out->validate();
  out->copyClientIdIfDefined(message);
  m_messageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
