
#include "SignupMessageConsumer.hh"
#include "SignupMessage.hh"

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
    handleSignup(signup.getUserName(), signup.getUserPassword(), signup.getFaction());
  }
}

void SignupMessageConsumer::handleSignup(const std::string &name,
                                         const std::string &password,
                                         const Faction &faction) const
{
  const auto playerDbId = m_signupService->trySignup(name, password, faction);

  if (!playerDbId)
  {
    warn("Failed to process signup message for player " + name);
  }

  auto message = std::make_unique<SignupMessage>(name, password, faction, playerDbId);
  message->validate();
  m_messageQueue->pushMessage(std::move(message));
}

} // namespace bsgo
