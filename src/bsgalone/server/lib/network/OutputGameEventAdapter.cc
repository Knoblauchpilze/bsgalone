
#include "OutputGameEventAdapter.hh"
#include "LoginMessage.hh"
#include "LogoutMessage.hh"
#include "PlayerLoginEvent.hh"
#include "PlayerLogoutEvent.hh"
#include "PlayerSignupEvent.hh"
#include "SignupMessage.hh"

namespace bsgalone::server {

OutputGameEventAdapter::OutputGameEventAdapter(ForManagingClientShPtr clientManager,
                                               core::IOutputNetworkAdapterPtr networkClient)
  : IGameEventListener()
  , ::core::CoreObject("game")
  , m_clientManager(std::move(clientManager))
  , m_networkClient(std::move(networkClient))
{
  setService("adapter");
  addModule("event");

  if (m_clientManager == nullptr)
  {
    throw std::invalid_argument("Expected non null client manager");
  }
  if (m_networkClient == nullptr)
  {
    throw std::invalid_argument("Expected non null network client");
  }
}

namespace {
const std::unordered_set<GameEventType> RELEVANT_EVENT_TYPES = {
  GameEventType::PLAYER_LOGIN,
  GameEventType::PLAYER_LOGOUT,
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
      publishLoginMessage(*m_networkClient, event.as<PlayerLoginEvent>());
      break;
    case GameEventType::PLAYER_LOGOUT:
      handleLogoutEvent(event.as<PlayerLogoutEvent>());
      break;
    case GameEventType::PLAYER_SIGNUP:
      publishSignupMessage(*m_networkClient, event.as<PlayerSignupEvent>());
      break;
    default:
      throw std::invalid_argument("Unsupported game event type " + str(event.type()));
  }
}

void OutputGameEventAdapter::handleLogoutEvent(const PlayerLogoutEvent &event)
{
  core::LogoutMessage message(event.getPlayerDbId());

  for (const auto &clientId : m_clientManager->getAllClients())
  {
    m_networkClient->sendMessage(clientId, message);
  }
}

} // namespace bsgalone::server
