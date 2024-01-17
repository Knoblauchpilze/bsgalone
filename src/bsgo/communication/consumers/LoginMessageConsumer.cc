
#include "LoginMessageConsumer.hh"
#include "LoginMessage.hh"

namespace bsgo {

LoginMessageConsumer::LoginMessageConsumer(const Services &services,
                                           IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("login", {MessageType::LOGIN})
  , m_loginService(services.login)
  , m_signupService(services.signup)
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_loginService)
  {
    throw std::invalid_argument("Expected non null login service");
  }
  if (nullptr == m_signupService)
  {
    throw std::invalid_argument("Expected non null signup service");
  }
  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void LoginMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &loginMessage = message.as<LoginMessage>();

  const auto operation = loginMessage.getLoginType();
  const auto status    = loginMessage.getLoginState();

  if (LoginType::LOGIN == operation && LoginState::PENDING == status)
  {
    handleLogin(loginMessage.getUserName(), loginMessage.getUserPassword());
  }
  if (LoginType::SIGNUP == operation && LoginState::PENDING == status)
  {
    handleSignup(loginMessage.getUserName(),
                 loginMessage.getUserPassword(),
                 *loginMessage.getFaction());
  }
}

void LoginMessageConsumer::handleLogin(const std::string &name, const std::string &password) const
{
  auto state          = LoginState::VALIDATED;
  const auto playerId = m_loginService->tryLogin(name, password);
  if (!playerId)
  {
    warn("Failed to process login message for player " + name);
    state = LoginState::REJECTED;
  }

  m_messageQueue->pushMessage(std::make_unique<LoginMessage>(LoginType::LOGIN, *playerId, state));
}

void LoginMessageConsumer::handleSignup(const std::string &name,
                                        const std::string &password,
                                        const Faction &faction) const
{
  auto state          = LoginState::VALIDATED;
  const auto playerId = m_signupService->trySignup(name, password, faction);
  if (!playerId)
  {
    warn("Failed to process signup message for player " + name);
    state = LoginState::REJECTED;
  }

  m_messageQueue->pushMessage(std::make_unique<LoginMessage>(LoginType::SIGNUP, *playerId, state));
}

} // namespace bsgo
