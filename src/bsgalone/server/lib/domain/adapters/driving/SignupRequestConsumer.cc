
#include "SignupRequestConsumer.hh"
#include "SignupRequest.hh"

namespace bsgalone::server {

SignupRequestConsumer::SignupRequestConsumer(ForExecutingSignupPtr signupUseCase)
  : ::core::CoreObject("signup")
  , m_useCase(std::move(signupUseCase))
{
  setService("consumer");

  if (nullptr == m_useCase)
  {
    throw std::invalid_argument("Expected non null signup use case");
  }
}

bool SignupRequestConsumer::isEventRelevant(const core::MessageType &type) const
{
  return type == core::MessageType::SIGNUP_REQUEST;
}

void SignupRequestConsumer::onEventReceived(const core::IMessage &message)
{
  const auto &request = message.as<core::SignupRequest>();

  const auto maybeClientId = request.tryGetClientId();
  if (!maybeClientId.has_value())
  {
    warn("Failed to process signup request without client identifier");
    return;
  }

  SignupData data{
    .username = request.getUsername(),
    .password = request.getPassword(),
    .faction  = request.getFaction(),
    .clientId = request.tryGetClientId().value(),
  };

  m_useCase->performSignup(data);
}

} // namespace bsgalone::server
