
#include "SignupMessageConsumer.hh"

namespace bsgo {

SignupMessageConsumer::SignupMessageConsumer(const Services &services,
                                             IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("signup", {MessageType::SIGNUP})
  , m_signupService(services.signup)
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_signupService)
  {
    throw std::invalid_argument("Expected non null signup service");
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

  const auto playerDbId = m_signupService->trySignup(name, password, faction);

  if (!playerDbId)
  {
    warn("Failed to process signup message for player " + name);
  }

  auto out = std::make_unique<SignupMessage>(name, password, faction, playerDbId);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_messageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
