
#include "SignupMessageConsumer.hh"

namespace bsgalone::server {

SignupMessageConsumer::SignupMessageConsumer(SignupServicePtr signupService,
                                             core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("signup", {core::MessageType::SIGNUP})
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

void SignupMessageConsumer::onEventReceived(const core::IMessage &message)
{
  const auto &signup = message.as<core::SignupMessage>();
  handleSignup(signup);
}

void SignupMessageConsumer::handleSignup(const core::SignupMessage &message) const
{
  const auto name     = message.getUserName();
  const auto password = message.getUserPassword();
  const auto faction  = message.getFaction();

  const auto maybePlayer = m_signupService->trySignup(name, password, faction);

  if (!maybePlayer.has_value())
  {
    warn("Failed to process signup message for player " + name);
  }

  auto out = std::make_unique<core::SignupMessage>(name,
                                                   password,
                                                   faction,
                                                   maybePlayer.has_value()
                                                     ? maybePlayer->dbId
                                                     : std::optional<core::Uuid>{});
  out->copyClientIdIfDefined(message);
  m_outputMessageQueue->pushEvent(std::move(out));
}

} // namespace bsgalone::server
