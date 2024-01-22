
#include "LoginMessageConsumer.hh"
#include "LoginMessage.hh"

namespace bsgo {

LoginMessageConsumer::LoginMessageConsumer(const Services &services,
                                           IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("login", {MessageType::LOGIN})
  , m_loginService(services.login)
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_loginService)
  {
    throw std::invalid_argument("Expected non null login service");
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
    handleLogin(login.getUserName(), login.getUserPassword());
  }
}

void LoginMessageConsumer::handleLogin(const std::string &name, const std::string &password) const
{
  const auto playerDbId = m_loginService->tryLogin(name, password);

  if (!playerDbId)
  {
    warn("Failed to process login message for player " + name);
  }

  auto message = std::make_unique<LoginMessage>(name, password, playerDbId);
  message->validate();
  m_messageQueue->pushMessage(std::move(message));
}

} // namespace bsgo
