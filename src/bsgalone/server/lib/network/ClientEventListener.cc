
#include "ClientEventListener.hh"

namespace bsgalone::server {

ClientEventListener::ClientEventListener(ClientManagerShPtr clientManager)
  : m_clientManager(std::move(clientManager))
{
  if (m_clientManager == nullptr)
  {
    throw std::invalid_argument("Expected non null client manager");
  }
}

bool ClientEventListener::isEventRelevant(const net::NetworkEventType &type) const
{
  return type == net::NetworkEventType::CLIENT_CONNECTED
         || type == net::NetworkEventType::CLIENT_DISCONNECTED;
}

void ClientEventListener::onEventReceived(const net::INetworkEvent &event)
{
  switch (event.type())
  {
    case net::NetworkEventType::CLIENT_CONNECTED:
      handleClientConnected(event.as<net::ClientConnectedEvent>());
      break;
    case net::NetworkEventType::CLIENT_DISCONNECTED:
      handleClientDisconnected(event.as<net::ClientDisconnectedEvent>());
      break;
    default:
      throw std::invalid_argument("Unsupported event type " + net::str(event.type()));
  }
}

void ClientEventListener::handleClientConnected(const net::ClientConnectedEvent &event)
{
  m_clientManager->registerClient(event.clientId());
}

void ClientEventListener::handleClientDisconnected(const net::ClientDisconnectedEvent &event)
{
  const auto maybePlayerId = m_clientManager->tryGetPlayerForClient(event.clientId());

  m_clientManager->removeClient(event.clientId());

  if (!maybePlayerId)
  {
    return;
  }

  // TODO: Restore automatic logout when client disconnects
  // and restore the associated tests
  // auto message = std::make_unique<core::LogoutMessage>(*maybePlayerId, true);
  // message->setClientId(event.clientId());
  // m_inputQueue->pushEvent(std::move(message));
}

} // namespace bsgalone::server
