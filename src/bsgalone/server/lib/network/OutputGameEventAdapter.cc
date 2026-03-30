
#include "OutputGameEventAdapter.hh"
#include "LoginMessage.hh"
#include "PlayerLoginEvent.hh"

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

bool OutputGameEventAdapter::isEventRelevant(const core::GameEventType &type) const
{
  return type == core::GameEventType::PLAYER_LOGIN;
}

namespace {
void publishLoginMessage(core::IOutputNetworkAdapter &adapter, const core::PlayerLoginEvent &event)
{
  // TODO: Client id could be removed.
  core::LoginMessage message(event.getClientId());
  if (event.successfulLogin())
  {
    message.setPlayerDbId(event.tryGetPlayerDbId().value());
    message.setRole(event.tryGetRole().value());
    message.setSystemDbId(event.tryGetSystemDbId().value());
  }

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
    default:
      throw std::invalid_argument("Unsupported game event type " + str(event.type()));
  }
}

} // namespace bsgalone::server
