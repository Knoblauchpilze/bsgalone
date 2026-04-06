
#include "LoginRequestConsumer.hh"
#include "LoginRequest.hh"

namespace bsgalone::server {

LoginRequestConsumer::LoginRequestConsumer(ForExecutingLoginPtr loginUseCase)
  : ::core::CoreObject("login")
  , m_useCase(std::move(loginUseCase))
{
  setService("consumer");

  if (nullptr == m_useCase)
  {
    throw std::invalid_argument("Expected non null login use case");
  }
}

bool LoginRequestConsumer::isEventRelevant(const core::MessageType &type) const
{
  return type == core::MessageType::LOGIN_REQUEST;
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

  LoginData data{
    .username = request.getUsername(),
    .password = request.getPassword(),
    .role     = request.getRole(),
    .clientId = request.tryGetClientId().value(),
  };

  m_useCase->performLogin(data);
}

} // namespace bsgalone::server
