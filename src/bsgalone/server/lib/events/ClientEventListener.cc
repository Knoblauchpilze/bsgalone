
#include "ClientEventListener.hh"
#include "LogoutMessage.hh"

namespace bsgalone::server {

ClientEventListener::ClientEventListener(ClientManagerShPtr clientManager,
                                         bsgo::IMessageQueueShPtr inputQueue)
  : m_manager(std::move(clientManager))
  , m_inputQueue(std::move(inputQueue))
{
  if (m_manager == nullptr)
  {
    throw std::invalid_argument("Expected non null client manager");
  }
  if (m_inputQueue == nullptr)
  {
    throw std::invalid_argument("Expected non null input queue");
  }
}

bool ClientEventListener::isEventRelevant(const net::EventType &type) const
{
  return type == net::EventType::CLIENT_CONNECTED || type == net::EventType::CLIENT_DISCONNECTED;
}

void ClientEventListener::onEventReceived(const net::IEvent &event)
{
  switch (event.type())
  {
    case net::EventType::CLIENT_CONNECTED:
      handleClientConnected(event.as<net::ClientConnectedEvent>());
      break;
    case net::EventType::CLIENT_DISCONNECTED:
      handleClientDisconnected(event.as<net::ClientDisconnectedEvent>());
      break;
    default:
      throw std::invalid_argument("Unsupported event type " + net::str(event.type()));
  }
}

void ClientEventListener::handleClientConnected(const net::ClientConnectedEvent &event)
{
  m_manager->registerClient(event.clientId());
}

void ClientEventListener::handleClientDisconnected(const net::ClientDisconnectedEvent &event)
{
  const auto maybePlayerId = m_manager->tryGetPlayerForClient(event.clientId());

  m_manager->removeClient(event.clientId());

  if (!maybePlayerId)
  {
    return;
  }

  auto message = std::make_unique<bsgo::LogoutMessage>(*maybePlayerId, true);
  message->setClientId(event.clientId());
  m_inputQueue->pushMessage(std::move(message));
}

} // namespace bsgalone::server
