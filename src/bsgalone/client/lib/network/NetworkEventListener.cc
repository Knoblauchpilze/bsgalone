
#include "NetworkEventListener.hh"
#include "ConnectionMessage.hh"

namespace bsgalone::client {

NetworkEventListener::NetworkEventListener(std::atomic_bool &connected,
                                           core::IMessageQueueShPtr inputQueue)
  : m_connected(connected)
  , m_inputQueue(std::move(inputQueue))
{
  if (m_inputQueue == nullptr)
  {
    throw std::invalid_argument("Expected non null input queue");
  }
}

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

  auto message = std::make_unique<core::ConnectionMessage>();
  m_inputQueue->pushEvent(std::move(message));
}

void NetworkEventListener::handleConnectionLost(const net::ConnectionLostEvent & /*event*/)
{
  m_connected.store(false);
}

} // namespace bsgalone::client
