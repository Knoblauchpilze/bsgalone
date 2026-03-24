
#include "PlayerLoginEventConsumer.hh"
#include "PlayerLoginEvent.hh"

namespace bsgalone::core {

PlayerLoginEventConsumer::PlayerLoginEventConsumer(ForPublishingLoginDataPtr useCase)
  : IGameEventListener()
  , m_useCase(std::move(useCase))
{
  if (m_useCase == nullptr)
  {
    throw std::invalid_argument("Expected non null use case");
  }
}

bool PlayerLoginEventConsumer::isEventRelevant(const GameEventType &type) const
{
  return type == GameEventType::PLAYER_LOGIN;
}

void PlayerLoginEventConsumer::onEventReceived(const IGameEvent &event)
{
  const auto &login = event.as<PlayerLoginEvent>();
  if (!login.successfulLogin())
  {
    return;
  }

  // TODO: This should include the rest of LoadingMessagesConsumer::handleLoginTransition
  // TODO: It should also publish the LoginMessage
  // TODO: Also in case the login failed
  m_useCase->publishLoginData(login.tryGetPlayerDbId().value());
}

} // namespace bsgalone::core
