
#include "ServerEventListener.hh"

namespace bsgalone::server {

ServerEventListener::ServerEventListener(std::atomic_bool &started)
  : m_started(started)
{}

bool ServerEventListener::isEventRelevant(const net::EventType &type) const
{
  return type == net::EventType::SERVER_STARTED || type == net::EventType::SERVER_STOPPED;
}

void ServerEventListener::onEventReceived(const net::IEvent &event)
{
  switch (event.type())
  {
    case net::EventType::SERVER_STARTED:
      handleServerStarted(event.as<net::ServerStartedEvent>());
      break;
    case net::EventType::SERVER_STOPPED:
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
