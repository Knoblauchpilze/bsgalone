
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
const std::unordered_set<core::GameEventType> RELEVANT_EVENT_TYPES = {
  core::GameEventType::PLAYER_LOGIN,
  core::GameEventType::PLAYER_SIGNUP,
};
}
bool OutputGameEventAdapter::isEventRelevant(const core::GameEventType &type) const
{
  return RELEVANT_EVENT_TYPES.contains(type);
}

namespace {
void publishLoginMessage(core::IOutputNetworkAdapter &adapter, const core::PlayerLoginEvent &event)
{
  core::LoginMessage message;
  if (event.successfulLogin())
  {
    message.setPlayerDbId(event.tryGetPlayerDbId().value());
    message.setRole(event.tryGetRole().value());
  }

  adapter.sendMessage(event.getClientId(), message);
}

void publishSignupMessage(core::IOutputNetworkAdapter &adapter, const core::PlayerSignupEvent &event)
{
  core::SignupMessage message(event.successfulSignup());
  adapter.sendMessage(event.getClientId(), message);
}
} // namespace

void OutputGameEventAdapter::onEventReceived(const core::IGameEvent &event)
{
  switch (event.type())
  {
    case core::GameEventType::PLAYER_LOGIN:
      publishLoginMessage(*m_networkClient, event.as<core::PlayerLoginEvent>());
      break;
    case core::GameEventType::PLAYER_SIGNUP:
      publishSignupMessage(*m_networkClient, event.as<core::PlayerSignupEvent>());
      break;
    default:
      throw std::invalid_argument("Unsupported game event type " + str(event.type()));
  }
}

} // namespace bsgalone::server
