
#include "SignupMessageConsumer.hh"

namespace bsgo {

SignupMessageConsumer::SignupMessageConsumer(SignupServicePtr signupService,
                                             IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("signup", {MessageType::SIGNUP})
  , m_signupService(std::move(signupService))
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_signupService)
  {
    throw std::invalid_argument("Expected non null signup service");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void SignupMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &signup = message.as<SignupMessage>();
  handleSignup(signup);
}

void SignupMessageConsumer::handleSignup(const SignupMessage &message) const
{
  const auto name     = message.getUserName();
  const auto password = message.getUserPassword();
  const auto faction  = message.getFaction();

  const auto maybePlayer = m_signupService->trySignup(name, password, faction);

  if (!maybePlayer.has_value())
  {
    warn("Failed to process signup message for player " + name);
  }

  auto out = std::make_unique<SignupMessage>(name,
                                             password,
                                             faction,
                                             maybePlayer.has_value() ? maybePlayer->id
                                                                     : std::optional<Uuid>{});
  out->copyClientIdIfDefined(message);
  m_outputMessageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
