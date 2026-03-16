
#include "NetworkEventListener.hh"
#include "LoginMessage.hh"

namespace bsgalone::client {

NetworkEventListener::NetworkEventListener(std::atomic_bool &connected,
                                           core::IOutputNetworkAdapterShPtr outputAdapter,
                                           std::optional<User> autoLogin)
  : m_connected(connected)
  , m_outputAdapter(std::move(outputAdapter))
  , m_autoLogin(std::move(autoLogin))
{
  if (m_outputAdapter == nullptr)
  {
    throw std::invalid_argument("Expected non null output adaptae");
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

  if (!m_autoLogin.has_value())
  {
    return;
  }

  core::LoginMessage login(m_autoLogin->role);
  login.setUserName(m_autoLogin->name);
  login.setPassword(m_autoLogin->password);

  m_outputAdapter->sendMessage(login);
}

void NetworkEventListener::handleConnectionLost(const net::ConnectionLostEvent & /*event*/)
{
  m_connected.store(false);
}

} // namespace bsgalone::client
