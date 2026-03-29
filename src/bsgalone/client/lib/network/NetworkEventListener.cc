
#include "NetworkEventListener.hh"
#include "LoginRequest.hh"

namespace bsgalone::client {

NetworkEventListener::NetworkEventListener(std::atomic_bool &connected)
  : m_connected(connected)
{}

bool NetworkEventListener::isEventRelevant(const net::NetworkEventType &type) const
{
  return type == net::NetworkEventType::CONNECTION_ESTABLISHED
         || type == net::NetworkEventType::CONNECTION_LOST;
}

void NetworkEventListener::onEventReceived(const net::INetworkEvent &event)
{
  switch (event.type())
  {
    case net::NetworkEventType::CONNECTION_ESTABLISHED:
      handleConnectionEstablished(event.as<net::ConnectionEstablishedEvent>());
      break;
    case net::NetworkEventType::CONNECTION_LOST:
      handleConnectionLost(event.as<net::ConnectionLostEvent>());
      break;
    default:
      throw std::invalid_argument("Unsupported event type " + net::str(event.type()));
  }
}

void NetworkEventListener::handleConnectionEstablished(
  const net::ConnectionEstablishedEvent & /*event*/)
{
  m_connected.store(true);
}

void NetworkEventListener::handleConnectionLost(const net::ConnectionLostEvent & /*event*/)
{
  m_connected.store(false);
}

} // namespace bsgalone::client
