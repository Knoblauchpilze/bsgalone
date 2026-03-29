
#include "ServerEventListener.hh"

namespace bsgalone::server {

ServerEventListener::ServerEventListener(std::atomic_bool &started)
  : m_started(started)
{}

bool ServerEventListener::isEventRelevant(const net::NetworkEventType &type) const
{
  return type == net::NetworkEventType::SERVER_STARTED
         || type == net::NetworkEventType::SERVER_STOPPED;
}

void ServerEventListener::onEventReceived(const net::INetworkEvent &event)
{
  switch (event.type())
  {
    case net::NetworkEventType::SERVER_STARTED:
      handleServerStarted(event.as<net::ServerStartedEvent>());
      break;
    case net::NetworkEventType::SERVER_STOPPED:
      handleServerStopped(event.as<net::ServerStoppedEvent>());
      break;
    default:
      throw std::invalid_argument("Unsupported event type " + net::str(event.type()));
  }
}

void ServerEventListener::handleServerStarted(const net::ServerStartedEvent & /*event*/)
{
  m_started.store(true);
}

void ServerEventListener::handleServerStopped(const net::ServerStoppedEvent & /*event*/)
{
  m_started.store(false);
}

} // namespace bsgalone::server
