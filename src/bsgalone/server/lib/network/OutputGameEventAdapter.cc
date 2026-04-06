
#include "OutputGameEventAdapter.hh"
#include "LoginMessage.hh"
#include "PlayerLoginEvent.hh"
#include "PlayerSignupEvent.hh"
#include "SignupMessage.hh"

namespace bsgalone::server {

OutputGameEventAdapter::OutputGameEventAdapter(core::IOutputNetworkAdapterPtr networkClient)
  : IGameEventListener()
  , m_networkClient(std::move(networkClient))
{
  if (m_networkClient == nullptr)
  {
    throw std::invalid_argument("Expected non null network client");
  }
}

namespace {
const std::unordered_set<GameEventType> RELEVANT_EVENT_TYPES = {
  GameEventType::PLAYER_LOGIN,
  GameEventType::PLAYER_SIGNUP,
};
}
bool OutputGameEventAdapter::isEventRelevant(const GameEventType &type) const
{
  return RELEVANT_EVENT_TYPES.contains(type);
}

namespace {
void publishLoginMessage(core::IOutputNetworkAdapter &adapter, const PlayerLoginEvent &event)
{
  core::LoginMessage message;
  if (event.successfulLogin())
  {
    message.setPlayerDbId(event.tryGetPlayerDbId().value());
    message.setRole(event.tryGetRole().value());
  }

  adapter.sendMessage(event.getClientId(), message);
}

void publishSignupMessage(core::IOutputNetworkAdapter &adapter, const PlayerSignupEvent &event)
{
  core::SignupMessage message(event.successfulSignup());
  adapter.sendMessage(event.getClientId(), message);
}
} // namespace

void OutputGameEventAdapter::onEventReceived(const IGameEvent &event)
{
  switch (event.type())
  {
    case GameEventType::PLAYER_LOGIN:
      // TODO: Should register in the client manager
      publishLoginMessage(*m_networkClient, event.as<PlayerLoginEvent>());
      break;
    case GameEventType::PLAYER_SIGNUP:
      publishSignupMessage(*m_networkClient, event.as<PlayerSignupEvent>());
      break;
    default:
      throw std::invalid_argument("Unsupported game event type " + str(event.type()));
  }
}

} // namespace bsgalone::server
