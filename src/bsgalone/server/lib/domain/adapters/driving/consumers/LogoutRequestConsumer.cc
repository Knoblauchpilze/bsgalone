
#include "LogoutRequestConsumer.hh"
#include "LogoutRequest.hh"

namespace bsgalone::server {

LogoutRequestConsumer::LogoutRequestConsumer(ForExecutingLogoutPtr logoutUseCase)
  : ::core::CoreObject("logout")
  , m_useCase(std::move(logoutUseCase))
{
  setService("consumer");

  if (nullptr == m_useCase)
  {
    throw std::invalid_argument("Expected non null logout use case");
  }
}

bool LogoutRequestConsumer::isEventRelevant(const core::MessageType &type) const
{
  return type == core::MessageType::LOGOUT_REQUEST;
}

void LogoutRequestConsumer::onEventReceived(const core::IMessage &message)
{
  const auto &request = message.as<core::LogoutRequest>();

  LogoutData data{
    .playerDbId = request.getPlayerDbId(),
  };

  auto success = withSafetyNet([this, &data]() { m_useCase->performLogout(data); }, "performLogout");
  if (!success)
  {
    warn("Failed to process logout request for player " + data.playerDbId.str());
  }
}

} // namespace bsgalone::server
